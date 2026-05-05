#include <Arduino.h>

#include "updater/command.hpp"
#include "updater/state.hpp"
#include "updater/movement.hpp"
#include "components/motor.hpp"
#include "components/ultrasons.hpp"
#include "components/bluetooth.hpp"
#include "constants.hpp"

unsigned long dernierMessagePi = 0;

// initialisation du state et tempfinAction
RobotState robotState = {IDLE, 0};

void setup()
{
  Serial.begin(115200);
  Serial.setTimeout(10);
  initBluetooth();
  initUltrasons();

  moteurStop();
}

void loop()
{
  updateBluetoothBridge();
  readCmd();

  updateUltrasons();

  robotState = updateState(robotState, distAv, dernierMessagePi);

  updateMovement(robotState);
}