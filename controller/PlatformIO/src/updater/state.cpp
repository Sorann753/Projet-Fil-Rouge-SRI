#include "updater/state.hpp"
#include "components/ultrasons.hpp"
#include "updater/command.hpp"

State currentState = IDLE;

extern unsigned long tempsFinAction;

void updateState() {
    if (distAv <= 20) {
        currentState = EMERGENCY;
        return;
    }

    if (currentCmd.active) {
        if (currentCmd.action == "STOP") {
            currentState = IDLE;
        } else if (currentState != EMERGENCY) {
            currentState = MOVING;
        }
    }
    
    if (currentState == EMERGENCY && distAv > 40) {
        currentState = IDLE;
    }
}