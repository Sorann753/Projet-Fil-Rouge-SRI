#include "controlMod/IControlMod.h"

IControlMod makeSimulController(){
    return (IControlMod){
        .init = NULL,
        .getAction = NULL,
        .updateState = NULL,
    };
}

IControlMod makeHardController(){
    return (IControlMod){
        .init = NULL,
        .getAction = NULL,
        .updateState = NULL,
    };
}