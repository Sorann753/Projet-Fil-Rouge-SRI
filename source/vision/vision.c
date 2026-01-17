#include "vision/vision.h"



double colorDistance(Color A, Color B){
    // this will be an extended euclidian distance in order to add
    // a stretching factor to improve recognition
    return 0.0;
}

ColorMasks extractColors(const Image* img, const ColorReferences colorRef){
    Matrix* masks = (Matrix*)malloc(sizeof(Matrix) * colorRef.colorCount);
    if(masks == NULL){
        //TODO : handle error case
    }

    for(int i = 0; i < colorRef.colorCount; i++){
        Matrix temp = initMatrix(img->lines, img->columns, sizeof(byte));
        if(!temp.isValid){
            // TODO : handle error case
        }
        memcpy(&(masks[i]), &temp, sizeof(Matrix));
        /** @note : masks[i] takes ownership of the data from temp */
    }

    for(int x = 0; x < img->columns; x++){
        for(int y = 0; y < img->lines; y++){
            // find the best distances
            double minDist = 999999999;
            uint8_t colorIndex = 0;

            for(int i = 0; i < colorRef.colorCount; i++){
                Color refPoint = colorRef.refPoints[i];
                Pixel p = getPixel(img, x, y);
                double dist = colorDistance(p, refPoint);

                if(dist < minDist){
                    minDist = dist;
                    colorIndex = i;
                }
            }

            // set the value in the corresponding mask to 1
            byte_matrixSet(&(masks[colorIndex]), x, y, 1);

            for(int i = 0; i < colorRef.colorCount; i++){
                // all the other masks take a 0 at this position
                if(i != colorIndex){ 
                    byte_matrixSet(&(masks[i]), x, y, 0);
                }
            }
        }
    }
}

Ball findSphere(const Matrix* colorMask){
    //fr
}

Cube findCube(const Matrix* colorMask){
    //fr
}

/**
 * @brief estimage the direction of the object compared to the robot
 * @param ObjectCenter the estimated coordinates of an object
 * @param delta how far from the center before it's not centered anymore
 * @return -1 if on the left, 0 if centered, 1 if on the right
 * 
 * @note this assume the coordinate system is zero at the center of the image
 * @note this function is estimating with a single measure and is very crude,
 * more precise estimations will be done later
 */
int8_t objectDirection(IntCoordinate ObjectCenter, uint32_t delta){
    if(ObjectCenter.x < -delta){
        return -1;
    }
    else if(ObjectCenter.x > delta){
        return 1;
    }
    else{
        return 0;
    }
}