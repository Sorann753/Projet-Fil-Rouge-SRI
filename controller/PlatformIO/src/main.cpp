#include <Arduino.h>

#include "updater/command.hpp"
#include "updater/state.hpp"
#include "updater/movement.hpp"
#include "components/motor.hpp"
#include "components/ultrasons.hpp"
#include "components/bluetooth.hpp"
#include "constants.hpp"




void setup() {
  Serial.begin(115200);

  initUltrasons();

  setspeedroue(robotspeed);

  moteurStop();
}

void loop() {

  readCmd(); 

  updateUltrasons();

  updateState();

  updateMovement();

}