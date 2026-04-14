#ifndef STATE_HPP
#define STATE_HPP

#include <Arduino.h>

enum State {
    IDLE,
    MOVING,
    EMERGENCY
};

extern unsigned long tempsFinAction;
extern State currentState;

/**
 * @brief Gère les transitions entre les états
 */
void updateState();

#endif