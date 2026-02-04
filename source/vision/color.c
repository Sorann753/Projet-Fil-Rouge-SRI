#include "vision/color.h"



HSVColor RGBtoHSV(Color base){
    // normalize
    double r = base.R / 255.0;
    double g = base.G / 255.0;
    double b = base.B / 255.0;

    double cmax = fmax(r, fmax(g, b));
    double cmin = fmin(r, fmin(g, b));
    double delta = cmax - cmin;

    HSVColor res = {
        .H = 0.0, // to be computed
        .S = 0.0, // to be computed
        .V = cmax,
    };

    if(delta == 0.0){
        // H and S has to be 0 so we can already return
        return res;
    }
    else if(cmax == r){
        res.H = 60 * (fmod(((g - b) / delta), 6));
    }
    else if(cmax == g){
        res.H = 60 * (((b - r) / delta) + 2);
    }
    else if(cmax == b){
        res.H = 60 * (((r - g) / delta) + 4);
    }
    else{
        // weird case, this should never happend
        assert(false);
    }

    if(cmax != 0.0){
        res.S = delta / cmax;
    }

    return res;
}

Color HSVtoRGB(HSVColor base){
    // TODO : unimplemented for now because it's not yet needed
    assert(false);
}

/**
 */
double colorDistance(HSVColor A, HSVColor B){
    double deltaH = fabs(B.H - A.H);
    if(deltaH > 180.0){ // we keep it in [0.0, 180.0]
        deltaH = 360.0 - deltaH;
    }
    // we normalize
    deltaH /= 180.0;

    // to scale deltaH to account for the saturation effect
    double midS = (A.S + B.S) / 2.0;

    double deltaS = B.S - A.S;

    // skip the sqrt because it's just for comparison
    return deltaS*deltaS + midS*deltaH*deltaH;
}

ColorMasks extractColors(const Image* img, const ColorReferences colorRef){
    Matrix* masks = (Matrix*)malloc(sizeof(Matrix) * colorRef.colorCount);
    if(masks == NULL){
        //TODO : handle error case
        assert(false);
    }

    for(int i = 0; i < colorRef.colorCount; i++){
        Matrix temp = initMatrix(img->lines, img->columns, sizeof(byte));
        if(!temp.isValid){
            // TODO : handle error case
            assert(false);
        }
        memcpy(&(masks[i]), &temp, sizeof(Matrix));
        /** @note : masks[i] takes ownership of the data from temp */
    }

    // we precompute the HSV of our reference points
    HSVColor* HSVReferences = (HSVColor*)malloc(sizeof(HSVColor) * colorRef.colorCount);
    if(HSVReferences == NULL){
        //TODO handle error case
        assert(false);
    }
    for(int i = 0; i < colorRef.colorCount; i++){
        HSVReferences[i] = RGBtoHSV(colorRef.refPoints[i]);
    }

    for(size_t x = 0; x < img->columns; x++){
        for(size_t y = 0; y < img->lines; y++){
            // find the best distances
            double minDist = 999999999;
            uint8_t colorIndex = 0;
            HSVColor p = RGBtoHSV(getPixel(img, x, y));

            for(uint8_t i = 0; i < colorRef.colorCount; i++){
                double dist = colorDistance(p, HSVReferences[i]);

                if(dist < minDist){
                    minDist = dist;
                    colorIndex = i;
                }
            }

            // set the value in the corresponding mask to 1
            byte_matrixSet(&(masks[colorIndex]), x, y, 1);

            for(uint8_t i = 0; i < colorRef.colorCount; i++){
                // all the other masks take a 0 at this position
                if(i != colorIndex){ 
                    byte_matrixSet(&(masks[i]), x, y, 0);
                }
            }
        }
    }

    free(HSVReferences);

    return (ColorMasks){ .masks = masks, .colorCount = colorRef.colorCount};
}

void freeColorMasks(ColorMasks* masks){
    for(uint8_t i = 0; i < masks->colorCount; i++){
        freeMatrix(&(masks->masks)[i]);
    }

    free(masks->masks);
    masks->masks = NULL;
}

/**
 * @brief reconstruct an image file from the color masks for visualization purposes
 */
bool exportMasks(const char* outFilePath, const ColorMasks masks, const ColorReferences ref){
    FILE* file = fopen(outFilePath, "wb");
    if(file == NULL){
        return false;
    }

    fwrite(&(masks.masks->columns), sizeof(size_t), 1, file);
    fwrite(&(masks.masks->lines), sizeof(size_t), 1, file);

    // use this matrix to format a buffer to be written to file later
    Matrix dataMatrix = initMatrix(masks.masks->lines, masks.masks->columns * 3, 1);
    if(!dataMatrix.isValid){
        fwrite("\0", 1, 1, file);
        fclose(file);
        return false;
    }
    byte* dataContent = (byte*)dataMatrix.content;
    
    for(size_t x = 0; x < masks.masks->columns; x++){
        for(size_t y = 0; y < masks.masks->lines; y++){
            // matrixGetIndex need the matrix for the columns and lines
            // but since they are identical between masks we can precompute it
            size_t index = matrixGetIndex(masks.masks, x, y);
            for(uint8_t k = 0; k < ref.colorCount; k++){
                const byte* maskContent = (byte*)(masks.masks[k].content);
                if(maskContent[index]){
                    dataContent[3 * index] = ref.refPoints[k].R;
                    dataContent[3 * index + 1] = ref.refPoints[k].G;
                    dataContent[3 * index + 2] = ref.refPoints[k].B;
                    break; // break out of that for once found
                }
            }
        }
    }

    //write the content of the image to the file
    size_t totalBytes = dataMatrix.lines * dataMatrix.columns;
    fwrite(dataContent, 1, totalBytes, file); 

    fclose(file);
    return true;
}