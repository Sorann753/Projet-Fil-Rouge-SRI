#ifndef AUTO_HPP
#define AUTO_HPP

#include <vector>
#include <variant>

#include "BiChannel.hpp"

#include "controller/arduinoController.hpp"

struct AutoPilot{
    AutoPilot(ArduinoController& hardware) : hardware(hardware)
    {}

    void readModel(){

    }

    void readOrders(){

    }

    void step(){
        // update currentGoal
        // find the best way to reach the goal
        // execute next action
    }

private:
    ArduinoController& hardware;
    int goal;
    std::vector<int> orders;
    int model;
};

struct ManualPilot{
    ManualPilot(ArduinoController& hardware) : hardware(hardware)
    {}

    void readModel(){}
    void readOrders(){
        //populate the order
    }

    void step(){
        // execute orders one after another
        // for(auto order : orders){

        // }
    }

private:
    std::vector<int> orders;
    ArduinoController hardware;
};

class Pilot{
public:
    template<typename T>
    Pilot(T controlSystem) : behavior(controlSystem)
    {}

    void readModel(BiChannel<int, int>& chan){
        std::visit([](auto& b){
            b.readModel();
        }, behavior);
    }
    void readOrders(BiChannel<int, int>& chan){
        std::visit([](auto& b){
            b.readOrders();
        }, behavior);
    }
    void step(){
        std::visit([](auto& b){
            b.step();
        }, behavior);
    }

private:
    std::variant<ManualPilot, AutoPilot> behavior;
};

#endif