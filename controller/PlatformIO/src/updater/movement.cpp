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
    if (currentCmd.action == "FORWARD")
      moteurAvancer();
    else if (currentCmd.action == "BACKWARD")
      moteurReculer();
    else if (currentCmd.action == "LEFT")
      moteurGauche();
    else if (currentCmd.action == "RIGHT")
      moteurDroite();
    break;
  }
}