#include "updater/movement.hpp"
#include "updater/state.hpp"
#include "updater/command.hpp"
#include "components/motor.hpp"
#include "constants.hpp"

void updateMovement() {
    switch (currentState) {
        case EMERGENCY:
        case IDLE:
            moteurStop();
            currentCmd.active = false; 
            break;

        case MOVING:
            if (currentCmd.active) {

                float ratio = (currentCmd.action == "FORWARD" || currentCmd.action == "BACKWARD") 
                              ? CM_PAR_SECONDE : DEG_PAR_SECONDE;
                
                tempsFinAction = millis() + (currentCmd.valeur / ratio) * 1000.0;

                if (currentCmd.action == "FORWARD") moteurAvancer();
                else if (currentCmd.action == "BACKWARD") moteurReculer();
                else if (currentCmd.action == "LEFT") moteurGauche();
                else if (currentCmd.action == "RIGHT") moteurDroite();

                currentCmd.active = false; 
            }
            break;
    }
}