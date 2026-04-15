#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <Arduino.h>

struct Command {
    String action;
    int valeur;
    bool active;
};

extern Command currentCmd;
void readCommand();

#endif
