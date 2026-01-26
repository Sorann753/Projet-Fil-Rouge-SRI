#include "vision/image.h"



ImageHeader parseHeader(const char* rawHeader){
    byte fieldID = 0;
    ImageHeader head;
    char field[FIELD_MAX_LENGTH+1];
    int fieldCaracterCounter = 0;
    ParsingState state = IMG_STATE_READ;

    for(int i = 0; i < HEADER_LENGTH || !done; i++){
        if(rawHeader[i] == ' ' || rawHeader[i] == '\n'){
            if(fieldCaracterCounter == 0){
                continue; // if the field buffer is still empty we just keep parsing
            }
            // terminate
            state = IMG_STATE_STOP;
        }
        else if(rawHeader[i] < 48 || rawHeader[i] > 57){ // Not A Number
            if(state == IMG_STATE_CUT){
                // commit the buffer
                state = IMG_STATE_READ;
                field[fieldCaracterCounter] = '\0';
                switch(fieldID){
                    case 0:
                        head.length = strtoul(field, NULL, 10);
                        break;
                    case 1:
                        head.height = strtoul(field, NULL, 10);
                        break;
                    case 2:
                        head.canalCount = strtoul(field, NULL, 10);
                        state = IMG_STATE_STOP; // done reading
                        break;
                    default:
                        state = IMG_STATE_STOP;
                }
                
                fieldID++;
                fieldCaracterCounter = 0;
            }
            // we just ignore the non numbers

        }
        else{ // valid number
            state = IMG_STATE_CUT;
            field[fieldCaracterCounter] = rawHeader[i];
            fieldCaracterCounter++;
        }
    }

    return head;
}

/**
 * @brief this read the first line of the file and extract the header of the image in RGB-txt format
 * @param source a file pointer to the start of the image file
 * @return an instance of the ImageHeader struct, in case of error all fields will be set to 0
 * 
 * @pre source is a valid pointer to the start of the file
 * @pre the first line fit within HEADER_MAX_LENGTH characters
 * @pre each fields fit within FIELD_MAX_LENGTH characters
 * @pre the fields are separated with spaces
 */
ImageHeader loadHeader(FILE* source){
    assert(source != NULL);

    /** @remark we assume that the first line fit within this buffer */
    char buffer[HEADER_MAX_LENGTH]; 

    if(fgets(buffer, sizeof(buffer), source) != NULL){
        // parsing the header
        return parseHeader(buffer);
    }
    else{
        // read error case
        return (ImageHeader){0, 0, 0};
    }
}



void getChunk(FILE* source, char* buffer, size_t bufferSize, size_t remaining){
    assert(!ferror(source));

    size_t charToRead = 0;
    if(remaining < bufferSize){
        charToRead = remaining+1;
    }
    else{
        charToRead = bufferSize-1;
    }

    char* success_ptr = fgets(buffer, charToRead, source);

    //if any error happened we mark the buffer and exit
    if(ferror(source) || success_ptr == NULL){ 
        buffer[0] = NAK; //error marker
        buffer[1] = '\0'; // just for safety
        return;
    }
}

/**
 * @note this function assume ASCII compatible files,
 * other characters may be read weirdly but won't crash anything (unlike scanf)
 */
Matrix channelLexer(FILE* source, ImageHeader head){
    assert(head.height > 0 && head.length > 0);

    Matrix channel = initMatrix(head.height, head.length, sizeof(byte));
    if(!channel.isValid){
        return ERROR_MATRIX;
    }
    byte* channelContent = (byte*)channel.content;
    const size_t channelLimit = head.height * head.length;
    size_t channelIndex = 0;

    char buffer[IMG_BUFFER_SIZE+1];
    ParsingState state = IMG_STATE_READ;
    char token[4] = {0, 0, 0, '\0'};
    byte tokenSize = 0;

    while (state != IMG_STATE_STOP){
        getChunk(source, buffer, IMG_BUFFER_SIZE+1, channelLimit);

        for(size_t i = 0; i < IMG_BUFFER_SIZE && state != IMG_STATE_STOP; i++){
            if(channelIndex == 89994){
                assert(channelIndex % 2 == 0);
            }
            if(buffer[i] == NAK){
                // ERROR while reading the file, we abort
                freeMatrix(&channel);
                return ERROR_MATRIX;
            }
            else if(channelIndex >= channelLimit){
                state = IMG_STATE_STOP;
            }
            else if(buffer[i] < 48 || buffer[i] > 57){ //not a number
                if(state == IMG_STATE_CUT && tokenSize != 0){ 
                    // we were reading a token and now we're out, so we commit the token
                    token[tokenSize] = '\0'; //mark the limit to the valid data
                    byte colorValue = (byte)strtoul(token, NULL, 10);
                    channelContent[channelIndex] = colorValue;
                    channelIndex++;
                    tokenSize = 0;

                    state = IMG_STATE_READ;
                }
                if(buffer[i] == '\0'){
                    //we just break this loop to request a new chunk
                    break; 
                }

                // ignore all of the non number characters
            } 
            else{ // buffer[i] in range [48, 57]
                state = IMG_STATE_CUT;
                token[tokenSize] = buffer[i];
                tokenSize++;

                if(tokenSize >= 3){
                    token[3] = '\0'; //just to make sure
                    byte colorValue = (byte)strtoul(token, NULL, 10);
                    channelContent[channelIndex] = colorValue;
                    channelIndex++;
                    tokenSize = 0;
                }
            }
        }
    }

    return channel;
}

/**
 *
 * @note this function assume 3 channel images
 */
void readImage(FILE* source, Image* img){
    assert(img->isValid);

    Pixel* imageContent = (Pixel*)(img->content);

    Matrix redChannel = channelLexer(source, (ImageHeader){img->columns, img->lines, 1});
    assert(redChannel.isValid);
    byte* redValues = (byte*)(redChannel.content);

    Matrix greenChannel = channelLexer(source, (ImageHeader){img->columns, img->lines, 1});
    assert(greenChannel.isValid);
    byte* greenValues = (byte*)(greenChannel.content);

    Matrix blueChannel = channelLexer(source, (ImageHeader){img->columns, img->lines, 1});
    assert(blueChannel.isValid);
    byte* blueValues = (byte*)(blueChannel.content);

    // check the dimensions
    assert(img->columns == redChannel.columns && img->lines == redChannel.lines);
    assert(img->columns == greenChannel.columns && img->lines == greenChannel.lines);
    assert(img->columns == blueChannel.columns && img->lines == blueChannel.lines);

    // we fill the image in place through the alias pointer
    for(size_t i = 0; i < img->columns; i++){
        for(size_t j = 0; j < img->lines; j++){
            size_t index = matrixGetIndex(img, i, j);
            imageContent[index].R = redValues[index];
            imageContent[index].G = greenValues[index];
            imageContent[index].B = blueValues[index];
        }
    }

    freeMatrix(&redChannel);
    freeMatrix(&greenChannel);
    freeMatrix(&blueChannel);
}


#include <malloc.h>
/**
 * @brief load an image in RBG-txt format
 * @param sourcePath a valid path to the file we need to read
 * @return a valid Image encoded as a Matrix of Pixel if the reading went well

 * OR
 * 
 * an invalid Image if any error occured, in which case the errno value shall be checked
 * and the Image not be used for anything
 */
Image loadImage(const char* sourcePath){
    assert(sourcePath != NULL);

    FILE* sourceFile = fopen(sourcePath, "r");
    if(sourceFile == NULL){ // could not open file
        // only on POSIX does fopen set the errno value to know what happened,
        // but this code is only intended for linux anyway
        return ERROR_MATRIX;
    }
    assert(ferror(sourceFile) == 0); // we just created it, there shouldn't be errors yet

    ImageHeader head = loadHeader(sourceFile);
    if(head.canalCount == 0){ //error case
        fclose(sourceFile);
        return ERROR_MATRIX;
    }
    assert(head.canalCount != 0 && head.height != 0 && head.length != 0);

    //! For now we only handle 3 channel images
    assert(head.canalCount == 3); 

    Image loadedImage = initMatrix(head.height, head.length, sizeof(Pixel));
    if(!loadedImage.isValid){ // couldn't allocate memory
        fclose(sourceFile);
        return ERROR_MATRIX;
    }

    readImage(sourceFile, &loadedImage);
    fclose(sourceFile);

    if(!loadedImage.isValid){
        return ERROR_MATRIX;
    }

    return loadedImage;
}



Pixel getPixel(const Image* const img, size_t X, size_t Y){
    size_t index = matrixGetIndex(img, X, Y);
    Pixel* ptr = (Pixel*)img->content;

    return ptr[index];
}

void setPixel(Image* const img, size_t X, size_t Y, Pixel p){
    size_t index = matrixGetIndex(img, X, Y);
    Pixel* ptr = ((Pixel*)(img->content));
    ptr[index] = p;
}

Image convolut(const Image* img, const Matrix byteMatrix){
    // TODO
    assert(false);
}



uint32_t packPixel(Pixel rawPixel){
    uint32_t packed = 0;

    packed |= rawPixel.R;
    packed |= rawPixel.G << 8;
    packed |= rawPixel.B << 16;

    /** @post packed is now in the format 0x00BBGGRR */

    return packed;
}

Pixel unpackPixel(uint32_t packedPixel){
    byte red    = packedPixel & 0x000000FF;
    byte green  = packedPixel & 0x0000FF00;
    byte blue   = packedPixel & 0x00FF0000;

    return (Pixel){
        .R = red,
        .G = green,
        .B = blue
    };
}
