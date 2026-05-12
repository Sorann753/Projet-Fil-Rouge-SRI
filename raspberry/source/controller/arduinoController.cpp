#include "controller/arduinoController.hpp"

ArduinoController::ArduinoController(){
    initSerial("/dev/ttyACM0");
    keepAlive();
}

ArduinoController::~ArduinoController(){
    closeArduino();
}

/**
 * @param dist how far to go in cm
 */
void ArduinoController::forward(int dist){
    sendCommand("F", dist);
    keepAlive();
}

/**
 * @param dist how far to go in cm
 */
void ArduinoController::backward(int dist){
    sendCommand("B", dist);
    keepAlive();
}

/**
 * @param alpha the angle to rotate in degrees
 */
void ArduinoController::turn(int alpha){
    sendCommand("R", alpha);
    keepAlive();
}

void ArduinoController::stop(){
    sendCommand("S", 0);
    keepAlive();
}

void ArduinoController::keepAlive(){

    if(kaTick % 100 == 0){
        sendCommand("PING", 0);
    }
    if(kaTick > 60'000) kaTick = 0;

    kaTick++;
}

std::pair<RobotData, bool> ArduinoController::getArduinoData(){
    // since this is a static char* being returned i really don't want a move to happen
    char* unsafeStaticUartMsg = readArduino();
    std::string UartMsg;
    if(unsafeStaticUartMsg != NULL){
        UartMsg = unsafeStaticUartMsg;
    }

    if(UartMsg.empty()){
        return std::make_pair(RobotData(), false);
    }

    RobotData dat;
    bool success = parseTelemetry(UartMsg.c_str(), &dat);

    return std::make_pair(dat, success);
}