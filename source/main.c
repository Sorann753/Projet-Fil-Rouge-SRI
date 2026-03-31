#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

//#define SIMULATOR
#define HARDWARE

#include "utils/matrix_impl.h"
#include "controlMod/IControlMod.h"
#include "actuator/IActuator.h"

#ifdef SIMULATOR
#include "history/history.h"
#include "controlMod/menu.h"
#endif

#ifdef HARDWARE
#endif



int main(void){
    #if defined (SIMULATOR) && defined (HARDWARE)
    #error you are not supposed to enable both at once
    #endif

    IActuator actuator;
    IControlMod controller;

    #ifdef SIMULATOR
    actuator = makeSimulActuator();
    controller = makeSimulController();
    #endif

    #ifdef HARDWARE
    actuator = makeHardActuator();
    controller = makeHardController();
    #endif

    if(actuator.init != NULL){
        actuator.init(NULL);
    }
    if(controller.init != NULL){
        controller.init();
    }

    return 0;
}
