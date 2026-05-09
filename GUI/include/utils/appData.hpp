#ifndef APPDATA_HPP
#define APPDATA_HPP

#include "windows/devOverlay.hpp"
#include <memory>

struct Polar {
    double r;
    double theta;
};

enum State {
    IDLE,
    MOVING,
    EMERGENCY
};

struct AppData
{
    State currentState;
    char currentCommand;
    int commandTime;
    int rightDist;
    int leftDist;
    int frontDist;
    size_t entityCount;
    bool devOverlayVisible = false;

};
extern AppData gApp;

extern std::vector<Polar> data;



#endif