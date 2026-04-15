#include "updater/movement.hpp"
#include "updater/state.hpp"
#include "updater/command.hpp"
#include "components/motor.hpp"
#include "constants.hpp"

#include "updater/movement.hpp"
#include "updater/state.hpp"
#include "updater/command.hpp"
#include "components/motor.hpp"
#include "constants.hpp"

void updateMovement() {
  switch (currentState) {
    case EMERGENCY:
      moteurStop();
      currentCmd.valeur = false;
      break;
    case IDLE:
      moteurStop();
      break;

    case MOVING:

      if (currentCmd.active) {

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
      }
      
      break;
  }
}