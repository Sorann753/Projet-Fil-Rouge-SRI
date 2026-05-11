#include "controller/arduinoController.hpp"

ArduinoController::ArduinoController(){
    initSerial("/dev/ttyACM0");
}

ArduinoController::~ArduinoController(){
    closeArduino();
}