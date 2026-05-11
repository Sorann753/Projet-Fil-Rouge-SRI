#ifndef WORLD_H
#define WORLD_H

#include "utils/position.h"

#include "vision/vision.h"

typedef struct UltraSound_t{
    float RightSideDist;
    float LeftSideDist;
    float FrontDist;
} UltraSound;

typedef struct Measure_t{
    Image view;
    UltraSound sideDist;
    Matrix pointCloud;
} Measure;

typedef struct Entity_t {
    /* data */
} Entity;



typedef struct World_t{
    RobotPosition location;
    Entity* ent;
} World;

#endif