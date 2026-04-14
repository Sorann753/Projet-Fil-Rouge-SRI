#include "updater/command.hpp"

Command currentCmd = {"", 0, false};

void readCommand() {
    if (Serial.available() > 0) {
        String input = Serial.readStringUntil('\n');
        input.trim();

        int sep = input.indexOf(':');
        currentCmd.action = (sep != -1) ? input.substring(0, sep) : input;
        currentCmd.valeur = (sep != -1) ? input.substring(sep + 1).toInt() : 0;
        currentCmd.active = true;
    }
}