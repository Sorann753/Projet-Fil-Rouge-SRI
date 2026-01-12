#include "vision/image.h"



ImageHeader parseHeader(const char* rawHeader){
    byte fieldID = 0;
    ImageHeader head;
    char field[FIELD_LENGTH];
    int fieldCaracterCounter = 0;
    bool done = false;

    for(int i = 0, last = 0; i < HEADER_LENGTH || !done; i++){
        if(rawHeader[i] == ' ' || rawHeader[i] == '\n'){
            if(fieldCaracterCounter == 0){
                continue; // if the field buffer is still empty we just keep parsing
            }

            // done reading the field
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
                    break;
                default:
                    done = true;
            }
            
            fieldID++;
            fieldCaracterCounter = 0;
        }
        else{
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
 * @pre the first line fit within HEADER_LENGTH characters
 * @pre each fields fit within FIELD_LENGTH characters
 * @pre the fields are separated with spaces
 */
ImageHeader loadHeader(FILE* source){
    assert(source != NULL);

    /** @remark we assume that the first line fit withing this buffer */
    char buffer[HEADER_LENGTH]; 

    if(fgets(buffer, sizeof(buffer), source) != NULL){
        // parsing the header
        return parseHeader(buffer);
    }
    else{
        // read error case
        return (ImageHeader){0, 0, 0};
    }
}



/**
 * @brief load an image in RBG-txt format
 * @param sourcePath a valid path to the file we need to read
 * @return a valid Image encoded as a Matrix of Pixel if the reading went well

 * OR
 * 
 * an invalid Image if any error occured, in which case the errno value shall be checked
 * and the Image not be used for anything
 * 
 * @warning this function uses strtok which is NOT thread safe, therefore this function isn't either
 */
Image loadImage(const char* sourcePath){
    assert(sourcePath != NULL);

    FILE* sourceFile = fopen(sourcePath, "r");
    if(sourceFile == NULL){ // could not open file
        // only on POSIX does fopen set the errno value to know what happened,
        // but this code is only intended for linux anyway
        return (Image){
            .columns = 0,
            .lines = 0,
            .content = NULL,
            .isValid = false
        };
    }
    assert(ferror(sourceFile) == 0);

    bool done = false;
    while(!done){
        char buffer[1024];
        size_t readChar = fread(buffer, sizeof(char), sizeof(buffer)-1, sourceFile);

        if(readChar == 0){ // either we finished that file or an error occured 
            if(feof(sourceFile)){
                done = true;
                break; //we're done and get out of that while loop
            }
            else if(ferror(sourceFile)){
                // something failed while reading that file so we return
                return (Image){
                    .lines = 0,
                    .columns = 0,
                    .content = NULL,
                    .isValid = false
                };
            }
            else {
                // weird situation occured ????
                assert(false); // should never happen
            }
        }

        buffer[readChar] = '\0'; // make sure it is null terminated no matter what
        for(size_t i = 0; i < readChar; i++){
            // WARNING : STRTOK IS NOT THREAD SAFE
            char* token = strtok(buffer, " ");
            while(token){
                // do something with that token
                
                token = strtok(NULL, " "); // tell strtok to get the next token
            }
        }
    }


    return (Image){
        .columns = 0,
        .lines = 0,
        .content = NULL,
        .isValid = false
    };
}





Pixel getPixel(const Image* const img, size_t X, size_t Y);

void setPixel(Image* const img, size_t X, size_t Y, Pixel p);



uint32_t packPixel(Pixel rawPixel){
    uint32_t packed = 0;

    packed |= rawPixel.R;
    packed |= rawPixel.G << 8;
    packed |= rawPixel.B << 16;

    /** @post packed is now in the format 0x00BBGGRR (lsb) */

    return packed;
}

Pixel unpackPixel(uint32_t packedPixel){
    byte red = packedPixel & 0x000000FF;
    byte green = packedPixel & 0x0000FF00;
    byte blue = packedPixel & 0x00FF0000;

    return (Pixel){
        .R = red,
        .G = green,
        .B = blue
    };
}
