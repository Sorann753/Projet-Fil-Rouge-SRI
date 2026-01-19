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

    return (ColorMasks){ .masks = masks, .colorCount = colorRef.colorCount};
}

void flagConnected(int x, int y, Matrix* flagMatrix, const Matrix* const mask){
    size_t index = matrixGetIndex(mask, x, y);
    const byte* srcBlocks = (const byte*)mask->content;
    byte* flags = (byte*)(flagMatrix->content);

    if(flags[index] == 1) return; // already visited
    if(srcBlocks[index] == 0) return; // outside of the object
    /** @post we are in the object at an unvisited point */

    flags[index] = 1;
    for(int i = -1; i < 2; i++){
        for(int j = -1; j < 2; j++){
            if(i == 0 && j == 0) continue; // skip yourself
            
            // flag the local neighbors
            flagConnected(x+i, y+j, flagMatrix, mask);
        }
    }
}

bool addObject(Objects* objectList, int X, int Y){
    if(objectList->count == objectList->capacity){
        // we need to extend the space in the array
        IntCoordinate* oldPoints = objectList->points;
        size_t oldCapacity = objectList->capacity;
        objectList->capacity *= 2;
        objectList->points = (IntCoordinate*)malloc(objectList->capacity * sizeof(IntCoordinate));
        if(objectList->points == NULL){
            //TODO : handle allocation error
            free(oldPoints);
            return false;
        }

        // copy the oldPoints to the new memory block
        memcpy(objectList->points, oldPoints, oldCapacity * sizeof(IntCoordinate));

        free(oldPoints);
        oldPoints = NULL;
    } /** @post we have enough space to add the new element */

    (objectList->points)[objectList->count] = (IntCoordinate){
        .x = X,
        .y = Y,
    };

    (objectList->count)++;
    return true;
}

Objects findObjects(const Matrix* const mask){
    Matrix flagMatrix = initMatrix(mask->lines, mask->columns, sizeof(byte));

    const byte* srcBlocks = (const byte*)mask->content;
    byte* flags = (byte*)flagMatrix.content;

    // zero the flag matrix to avoid UB
    for(int x = 0; x < mask->columns; x++){
        for(int y = 0; y < mask->lines; y++){
            size_t index = matrixGetIndex(&flagMatrix, x, y);
            flags[index] = 0;
        }
    } /** @post all elements of the flagMatrix are valid */

    Objects connectedElements = {
        .points = (IntCoordinate*)malloc(5 * sizeof(IntCoordinate)),
        .count = 0,
        .capacity = 5,
    };
    if(connectedElements.points == NULL){
        // memory allocation failed, we abort
        freeMatrix(&flagMatrix);
        connectedElements.capacity = 0;
        return connectedElements;
    }

    for(int x = 0; x < mask->columns; x++){
        for(int y = 0; y < mask->lines; y++){
            size_t index = matrixGetIndex(mask, x, y);
            // check that this pixel isn't flagged already
            if(flags[index] == 1) continue; // already known
            if(srcBlocks[index] == 0) continue; // not in an object

            // flag all the connected pixels
            flagConnected(x, y, &flagMatrix, mask);

            // commit the object to the array
            bool success = addObject(&connectedElements, x, y);
            if(!success){
                // TODO : handle failure case
            }
        }
    }

    // cleanup
    freeMatrix(&flagMatrix);

    return connectedElements;
}

Ball findSphere(const Matrix* colorMask){
    Objects things = findObjects(colorMask);
    
}

Cube findCube(const Matrix* colorMask){
    // TODO
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