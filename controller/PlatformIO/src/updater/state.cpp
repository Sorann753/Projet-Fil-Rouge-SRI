#include "updater/state.hpp"
#include "components/ultrasons.hpp"
#include "updater/command.hpp"

State currentState = IDLE;

extern unsigned long tempsFinAction;

void updateState() {
    unsigned long actuelMillis = millis();

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

    if (currentState == MOVING && actuelMillis >= tempsFinAction) {
        currentState = IDLE;
    }

    if (currentState == EMERGENCY && distAv > 40) {
        currentState = IDLE;
    }

}