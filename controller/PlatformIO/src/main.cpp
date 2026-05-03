#include <Arduino.h>

#include "updater/command.hpp"
#include "updater/state.hpp"
#include "updater/movement.hpp"
#include "components/motor.hpp"
#include "components/ultrasons.hpp"
#include "components/bluetooth.hpp"
#include "constants.hpp"

unsigned long dernierMessagePi = 0;
void setup()
{
  Serial.begin(115200);

  initUltrasons();

  moteurStop();
}

void loop()
{

  readCmd();

  updateUltrasons();

  updateState();

  updateMovement();
}