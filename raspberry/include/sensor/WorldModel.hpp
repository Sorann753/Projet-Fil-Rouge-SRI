#ifndef WORLD_MODEL_HPP
#define WORLD_MODEL_HPP

#include <vector>

#include "utils/position.h"

class Objects{};


class WorldMap{
public:
    void update(PolarCoordinate* raw, size_t pointCount){

    }
private:
};

struct WorldModel{
    std::vector<Objects> visibleObjects;
    WorldMap map;
    RobotPosition pos;
};

#endif // WORLD_MODEL_HPP
