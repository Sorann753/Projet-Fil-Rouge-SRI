#include "actuator/IActuator.h"

IActuator makeSimulActuator(){
    return (IActuator){
        .init = NULL,
        .read_config = NULL,
        .forward = NULL,
        .backward = NULL,
        .turn = NULL,
    };
}

IActuator makeHardActuator(){
    return (IActuator){
        .init = NULL,
        .read_config = NULL,
        .forward = NULL,
        .backward = NULL,
        .turn = NULL,
    };
}