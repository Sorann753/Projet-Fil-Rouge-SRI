#ifndef CARTOGRAPHY_H
#define CARTOGRAPHY_H

#include "stdlib.h"

#include "utils/position.h"

typedef struct Cluster_t{
    PolarCoordinate* containedPoints;
    size_t pointCount;
} Cluster;

typedef enum ShapeKind_t{
    LINE_SHAPE,
    ROUND_SHAPE,
    RECT_SHAPE,
    OTHER_SHAPE
} ShapeKind;

typedef struct Shape_t{
    Cluster raw;
    Coordinate center;
    float length; // length or radius
    float alpha; // angle or unused
    ShapeKind kind;
} Shape;

Cluster* makeClusters(PolarCoordinate* rawPoints, size_t count);

ShapeKind recognizeShape(Cluster obj);
Coordinate getCenter(Cluster obj);
Shape fitLine(Cluster obj);
Shape fitRect(Cluster obj);
Shape fitCirle(Cluster obj);
Shape* makeShapes(Cluster* clusters, size_t count);

#endif