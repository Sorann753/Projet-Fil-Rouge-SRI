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

  readCommand(); 

  updateUltrasons();

  updateState();
  
  updateMovement();

  if (millis() - dernierMessagePi >= timeoutPi && currentState == MOVING) {
      currentState = IDLE;
  }

  delay(50);
}