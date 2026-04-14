#include <Arduino.h>
#include "components/motor.hpp"
#include "updater/command.hpp"
#include "updater/state.hpp"
#include "updater/movement.hpp"
#include "components/ultrasons.hpp"
#include "constants.hpp"

unsigned long tempsFinAction = 0;
unsigned long dernierMessagePi = 0;

void setup() {
  Serial.begin(115200);
  initUltrasons();
  setspeedroue(robotspeed);
  moteurStop();
}

void loop() {
  // 1. Écouter (Input)
  readCommand(); 
  updateUltrasons();

  // 2. Décider (Logic)
  updateState();

  // 3. Agir (Output)
  updateMovement();

  // 4. Watchdog (Global)
  if (millis() - dernierMessagePi >= timeoutPi && currentState == MOVING) {
      currentState = IDLE;
  }
}