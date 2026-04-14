#include "updater/movement.hpp"
#include "updater/state.hpp"
#include "updater/command.hpp"
#include "components/motor.hpp"
#include "constants.hpp"

void updateMovement() {
  switch (currentState) {
    case EMERGENCY:
        moteurStop();
        currentCmd.active = false;
        break;
    case IDLE:
      moteurStop();
      currentCmd.active = false;
      break;

    case MOVING:
      if (currentCmd.active) {
        float ratio = 1.0;

        // Sélection du ratio selon le type de mouvement
        if (currentCmd.action == "FORWARD") {
          ratio = CM_PAR_SECONDE;
        } 
        else if (currentCmd.action == "BACKWARD") {
          ratio = CM_PAR_SECONDE;
        } 
        else {
          ratio = DEG_PAR_SECONDE;
        }

        tempsFinAction = millis() + (currentCmd.valeur / ratio) * 1000.0;

        if (currentCmd.action == "FORWARD") {
          moteurAvancer();
        } 
        else if (currentCmd.action == "BACKWARD") {
          moteurReculer();
        } 
        else if (currentCmd.action == "LEFT") {
          moteurGauche();
        } 
        else if (currentCmd.action == "RIGHT") {
          moteurDroite();
        }

        currentCmd.active = false;
      }
      break;
  }
}