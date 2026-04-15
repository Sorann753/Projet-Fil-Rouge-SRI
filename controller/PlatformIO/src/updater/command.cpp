#include "updater/command.hpp"
#include "constants.hpp"

Command currentCmd = {"", 0, false};

void readCommand() {
  if (Serial.available() > 0) {
    extern unsigned long dernierMessagePi;
    dernierMessagePi = millis();

    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    int index_separateur = cmd.indexOf(':');
    String action = cmd;
    int valeur = 0;

    if (index_separateur != -1) {
      action = cmd.substring(0, index_separateur);
      valeur = cmd.substring(index_separateur + 1).toInt();
    }
    
    currentCmd.action = action;
    currentCmd.valeur = valeur;
    currentCmd.active = true;
  }
}