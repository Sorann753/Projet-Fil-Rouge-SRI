#include "constants.hpp"
#include "updater/state.hpp"
#include "components/ultrasons.hpp"
#include "updater/command.hpp"

State currentState = IDLE;

unsigned long tempsFinAction = 0;

void updateState() {
    unsigned long tempsActuel = millis(); 
    static State lastState = IDLE;
    
    if (distAv <= 20) {
        currentState = EMERGENCY;
        return;
    }

    if (currentCmd.active) {
        if (currentCmd.action == "STOP") {
            currentState = IDLE;
        } 
        else if (currentState != EMERGENCY) {
            currentState = MOVING;
            
            float ratio = 1.0;
            if (currentCmd.action == "FORWARD") { ratio = CM_PAR_SECONDE; } 
            else if (currentCmd.action == "BACKWARD") { ratio = CM_PAR_SECONDE; } 
            else { ratio = DEG_PAR_SECONDE; }

            tempsFinAction = tempsActuel + (currentCmd.valeur / ratio) * 1000.0;
            
        }
    }

    if (currentState == MOVING) {
        if (tempsActuel >= tempsFinAction) {
            currentState = IDLE;
            tempsFinAction = 0; 
            Serial.println("Temps ecoule -> Retour IDLE");
        }
    }

    if (currentState == EMERGENCY && distAv > 40) {
        currentState = IDLE;
    }

    if (currentState != lastState)
    {
        Serial.print("CHANGMEMENT D'ÉTAT = " + currentState);
        lastState = currentState;
    }
}