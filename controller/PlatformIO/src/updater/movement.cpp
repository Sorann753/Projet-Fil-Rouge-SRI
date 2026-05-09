#include "updater/movement.hpp"
#include "updater/state.hpp"
#include "updater/command.hpp"
#include "components/motor.hpp"
#include "constants.hpp"

void updateMovement(RobotState robotState)
{
  switch (robotState.state)
  {
  case EMERGENCY:
  case IDLE:
    moteurStop();
    break;

  case MOVING:
    if (currentCmd.action == "F")
      moteurAvancer();
    else if (currentCmd.action == "B")
      moteurReculer();
    else if (currentCmd.action == "L")
      moteurGauche();
    else if (currentCmd.action == "R")
      moteurDroite();
    break;
  }
}