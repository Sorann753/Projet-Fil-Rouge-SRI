#ifndef ARDUINO_CONTROLLER_HPP
#define ARDUINO_CONTROLLER_HPP

#include <string>
#include <utility>

#include "rbiArduinoInterface/arduino_interface.h"

class ArduinoController{
public:
    ArduinoController();
    ~ArduinoController();

    /**
     * @param dist how far to go in cm
     */
    void forward(int dist);

    /**
    * @param dist how far to go in cm
    */
    void backward(int dist);

    /**
    * @param alpha the angle to rotate in degrees
    */
    void turn(int alpha);

    void stop();

    void keepAlive();

    std::pair<RobotData, bool> getArduinoData();

private:
    unsigned int kaTick = 0;
};

#endif