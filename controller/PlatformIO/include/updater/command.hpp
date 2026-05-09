#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <Arduino.h>

struct Command
{
    String action;
    int valeur;
    bool active;
};

extern unsigned long dernierMessagePi; // le temp de reception du dernier message par la raspbery pi

extern Command currentCmd; // la commande actuelle
extern Command listCmd[];  // une liste de commande a executer dans lordre

extern int indexRead;  // lire la commande
extern int indexWrite; // ecrire une nouvelle commande

void readCmd();
void writeCmd(Command commande);

#endif
