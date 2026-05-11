#ifndef IACTUATOR_H
#define IACTUATOR_H

#include "stdlib.h"

typedef struct IActuator_t{
    void (*init)();
    void (*read_config)(void*);
    void (*forward)(float);
    void (*backward)(float);
    void (*turn)(float);
} IActuator;

IActuator makeHardActuator();

IActuator makeSimulActuator();

#endif