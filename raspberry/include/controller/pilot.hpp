#ifndef AUTO_HPP
#define AUTO_HPP

#include <iostream>
#include <vector>
#include <variant>

#include "BiChannel.hpp"

#include "controller/arduinoController.hpp"

enum AutoState{
    IDLE,
    BorderSearch
};

struct AutoPilot{
    AutoPilot(ArduinoController& hardware) : hardware(hardware)
    {}

    void readModel(BiChannel<int, int>& sensorChan){
        if(sensorChan.outputEmpty()){
            return;
        }

        model = sensorChan.popOut();
    }

    void readOrders(BiChannel<int, int>& netChan){
        if(netChan.outputEmpty()){
            return;
        }

        //TODO
    }

    void step(){
        // update currentGoal
        // find the best way to reach the goal
        // execute next action

        if(state == AutoState::IDLE){
            return;
        }

        if(state == AutoState::BorderSearch){
            if(model > DIST_LIMITE_AVANT){
                std::cout << "[DEBUG] forward" << std::endl;
                hardware.forward(30);
            }
            else{
                std::cout << "[DEBUG] obstacle" << std::endl;
                hardware.backward(30);
                hardware.turn(90);
            }
        }

        hardware.keepAlive();
    }

private:
    AutoState state = AutoState::BorderSearch;
    ArduinoController& hardware;
    int goal; //TODO
    std::vector<int> orders; //TODO
    int model; //TODO
};

struct ManualPilot{
    ManualPilot(ArduinoController& hardware) : hardware(hardware)
    {}

    void readModel(BiChannel<int, int>& sensorChan){}
    void readOrders(BiChannel<int, int>& netChan){
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
        std::visit([&](auto& b){
            b.readModel(chan);
        }, behavior);
    }
    void readOrders(BiChannel<int, int>& chan){
        std::visit([&](auto& b){
            b.readOrders(chan);
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