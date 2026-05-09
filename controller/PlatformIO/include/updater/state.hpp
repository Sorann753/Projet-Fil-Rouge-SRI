#ifndef STATE_HPP
#define STATE_HPP

#include <Arduino.h>

enum State
{
    IDLE,
    MOVING,
    EMERGENCY
};

struct RobotState
{
    State state;
    unsigned long tempsFinAction;
};

/**
 * @brief Gère les transitions entre les états
 */
RobotState updateState(RobotState current, int distAv, unsigned long dernierMessagePi);

#endif