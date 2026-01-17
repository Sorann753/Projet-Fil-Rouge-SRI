#ifndef VISION_H
#define VISION_H

#include "image.h"
#include "utils/position.h"

typedef struct ColorReferences_t{
    Color* refPoints;
    uint8_t colorCount;
} ColorReferences;

typedef struct ColorMasks_t{
    Matrix* masks;
    uint8_t colorCount;
} ColorMasks;

typedef struct Ball_t{
    IntCoordinate center;
    uint32_t radius;
} Ball;

typedef struct Cube_t{
    IntCoordinate center;
} Cube;

double colorDistance(Color A, Color B);

ColorMasks extractColors(const Image* img, const ColorReferences colorRef);

Ball findSphere(const Matrix* colorMask);

Cube findCube(const Matrix* colorMask);

/**
 * @brief estimage the direction of the object compared to the robot
 * @param ObjectCenter the estimated coordinates of an object
 * @param delta how far from the center before it's not centered anymore
 * @return -1 if on the left, 0 if centered, 1 if on the right
 * 
 * @note this assume the camera is aligned with the robot
 * @note this function is estimating with a single image and is very crude,
 * more precise estimations will be done later
 */
int8_t objectDirection(IntCoordinate ObjectCenter, uint32_t delta);

#endif
