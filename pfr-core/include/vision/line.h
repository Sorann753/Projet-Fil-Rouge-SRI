#ifndef VISION_LINE_H
#define VISION_LINE_H

#include <stdlib.h>

#include "utils/position.h"
#include "image.h"

typedef struct Vector2d_t {
    double vx;
    double vy;
} Vector2d;

typedef struct MathLine_t {
    IntCoordinate point;
    Vector2d direct;
} MathLine;



IntCoordinate findMiddle(IntCoordinate A, IntCoordinate B);

/**
 * @brief construct a normalised vector is a random direction
 */
Vector2d makeRandomVec(void);

double getVecLength(Vector2d v);

MathLine makeLine(IntCoordinate refPoint, Vector2d directingVec);

/**
 * @brief construct a line perpendicular to the first and passing through the rotation point
 * @param base what we want to be perpendicular to
 * @param crossingPoint the point where both lines intersect,
 * if the pointer is NULL then the reference point of the base line is used
 */
MathLine perpendicular(MathLine base, const IntCoordinate* crossingPoint);

#endif