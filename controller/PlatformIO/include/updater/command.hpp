#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <Arduino.h>

struct Command {
    String action;
    int valeur;
    bool active;
};

extern unsigned long dernierMessagePi;
extern Command currentCmd;
extern Command listCmd[];
extern int indexRead;   
extern int indexWrite; 

void readCmd();
void writeCmd(Command commande);

#endif
