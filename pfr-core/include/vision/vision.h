#ifndef VISION_H
#define VISION_H

#include <time.h>
#include <stdlib.h>

#include "utils/position.h"
#include "configLoader/configLoader.h"
#include "manualPilot/command.h"

#include "line.h"
#include "image.h"
#include "color.h"

//TODO : use config file to load them dynamically
#define SAMPLE_POINT_COUNT 5
#define OBJECT_MIN_SURFACE 100



typedef struct Object_t {
    IntCoordinate point; // some point on the object
    size_t surface; // how many pixel compose this object
} Object;

typedef struct Objects_t{
    Object* elements; 
    size_t count;
    size_t capacity;
} Objects;

typedef struct ObjectFeature_t {
    // the pixel of another color inside the bounds
    size_t holePixelCount; 

    // the pixel of the same color outside the bounds
    size_t artifactPixelCount; 
} ObjectFeature;

typedef struct Ball_t{
    IntCoordinate center;
    uint32_t radius;
} Ball;

typedef struct BallArray_t{
    Ball* balls;
    size_t count;
    size_t capacity;
} BallArray;

typedef struct Cube_t{
    IntCoordinate center;
} Cube;



/**
 * @brief perform the necessary initializations for the vision module
 */
void initVision(void);



/**
 * @return true if success, false in case of failure
 */
bool addObject(Objects* objectList, long X, long Y, size_t surface);

void flagConnected(long x, long y, Matrix* flagMatrix, const Matrix* const mask);

size_t getObjectSurface(long x, long y, const Matrix* mask, Matrix* flagMatrix, size_t depthLimit);

/**
 * @brief find a point from each of the connected elements
 * @param mask a byte matrix
 * @return a list of a point within each of the connected elements
 */
Objects findObjects(const Matrix* const mask);


void traceChord(const Matrix* colorMask, MathLine line, IntCoordinate* forwardSearch, IntCoordinate* backwardSearch);

/**
 * @brief trace the line until it goes out of the object in forward and in backward
 * then return the middle point
 */
IntCoordinate findCenterIntersect(const Matrix* colorMask, MathLine line);

double findRadius(const Matrix* colorMask, MathLine line);

bool addBall(BallArray* ballList, Ball newBall);

ObjectFeature modelFeatures(IntCoordinate current, const Matrix* colorMask, Ball model, Matrix* flagMatrix, size_t depthLimit);

BallArray findSpheres(const Matrix* colorMask);



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
direction_t objectDirection(IntCoordinate ObjectCenter, int32_t delta);

bool exportBallsToFile(const char* outPath, BallArray balls);

#endif
