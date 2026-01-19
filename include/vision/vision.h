#ifndef VISION_H
#define VISION_H

#include "image.h"
#include "utils/position.h"
#include "manualPilot/command.h"

typedef struct ColorReferences_t{
    Color* refPoints;
    uint8_t colorCount;
} ColorReferences;

typedef struct ColorMasks_t{
    Matrix* masks;
    uint8_t colorCount;
} ColorMasks;

typedef struct Objects_t{
    IntCoordinate* points; // the points on each objects
    size_t count;
    size_t capacity;
} Objects;

typedef struct Ball_t{
    IntCoordinate center;
    uint32_t radius;
} Ball;

typedef struct Cube_t{
    IntCoordinate center;
} Cube;

double colorDistance(Color A, Color B);

ColorMasks extractColors(const Image* img, const ColorReferences colorRef);

/**
 * @return true if success, false in case of failure
 */
bool addObject(Objects* objectList, int X, int Y);

void flagConnected(int x, int y, Matrix* flagMatrix, const Matrix* const mask);

/**
 * @brief find a point from each of the connected elements
 * @param mask a byte matrix
 * @return a list of a point within each of the connected elements
 */
Objects findObjects(const Matrix* const mask);

Ball findSphere(const Matrix* colorMask);

Cube findCube(const Matrix* colorMask);

/**
 * @brief estimage the direction of the object compared to the robot
 * @param ObjectCenter the estimated coordinates of an object
 * @param delta how far from the center before it's not centered anymore
 * @return the relative direction of the object, DIR_NONE being used for centered
 * 
 * @note this assume the camera is aligned with the robot
 * @note this function is estimating with a single image and is very crude,
 * more precise estimations will be done later
 */
direction_t objectDirection(IntCoordinate ObjectCenter, uint32_t delta);

#endif
