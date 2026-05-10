#ifndef CARTOGRAPHY_H
#define CARTOGRAPHY_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "utils/position.h"
#include "utils/matrix.h"

typedef struct Cluster_t{
    PolarCoordinate* containedPoints;
    Coordinate* cartesianPoints;
    size_t pointCount;
} Cluster;

typedef enum ShapeKind_t{
    LINE_SHAPE,
    ROUND_SHAPE,
    ANGLE_SHAPE,
    OTHER_SHAPE
} ShapeKind;

typedef struct Shape_t{
    Cluster raw;
    Coordinate center;
    float length; // length or radius
    float alpha; // angle or unused
    ShapeKind kind;
} Shape;

Cluster* makeClusters(PolarCoordinate* rawPoints, size_t pointCount , size_t* clusterPoints);

/**
 * @brief get the curvature angle at B
 * @return the signed angle curved such that
 * 0 if straight line,
 * pi if turn left
 * -pi if turn right
 * and all the in-between
 */
double getCurvature(Coordinate A, Coordinate B, Coordinate C);

Coordinate getCenter(Cluster obj);
ShapeKind recognizeShape(Cluster obj);
Shape fitLine(Cluster obj);
Shape fitRect(Cluster obj);
Shape fitCirle(Cluster obj);
Shape* makeShapes(Cluster* clusters, size_t count);

#endif