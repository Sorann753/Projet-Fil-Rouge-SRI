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

    if (currentState == IDLE) {
        if (listCmd[indexRead].active) {
            currentCmd = listCmd[indexRead];
            listCmd[indexRead].active = false;
            indexRead = (indexRead + 1) % 10;

            if (currentCmd.action == "STOP") {
                currentState = IDLE;
            } 
            else if (currentState != EMERGENCY) {
                currentState = MOVING;
                
                float ratio = 1.0;
                if (currentCmd.action == "FORWARD") {
                    ratio = CM_PAR_SECONDE;
                } 
                else if (currentCmd.action == "BACKWARD") {
                    ratio = CM_PAR_SECONDE;
                } 
                else {
                    ratio = DEG_PAR_SECONDE;
                }

                tempsFinAction = tempsActuel + (currentCmd.valeur / ratio) * 1000.0;
            }
        }
    }

    if (currentState == MOVING) {
        if (tempsActuel >= tempsFinAction) {
            currentState = IDLE;
            Serial.println("Temps de commande :" + tempsFinAction);
            Serial.println("Temps ecoule -> Retour IDLE");
            tempsFinAction = 0; 
            
        }
    }

    if (currentState == EMERGENCY && distAv > 40) {
        currentState = IDLE;
    }

    if (currentState != lastState) {
        Serial.print("CHANGEMENT D'ETAT = ");
        Serial.println(currentState);
        lastState = currentState;
    }
}