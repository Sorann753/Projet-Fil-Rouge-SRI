#ifndef ICONTROL_MOD_H
#define ICONTROL_MOD_H

#include "stdlib.h"

#include "manualPilot/command.h"

typedef struct IControlMod_t
{
    void (*init)(void);
    void (*getAction)(command_list);
    void (*updateState)(void *);
} IControlMod;

IControlMod makeHardController();

IControlMod makeSimulController();

#endif