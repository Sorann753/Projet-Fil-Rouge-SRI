#include <Arduino.h>

#include "updater/command.hpp"
#include "updater/state.hpp"
#include "updater/movement.hpp"
#include "components/motor.hpp"
#include "components/ultrasons.hpp"
#include "components/bluetooth.hpp"
#include "constants.hpp"

unsigned long tempsFinAction = 0;
unsigned long dernierMessagePi = 0;

void setup() {
  Serial.begin(115200);
  initBluetooth();

  initUltrasons();

  setspeedroue(robotspeed);
  moteurStop();
}

void loop() {

  readCommand(); 

  Serial1.print("Cmd:");
  Serial1.println(currentCmd.action);

  updateBluetoothBridge();

  updateUltrasons();

  updateState();
  Serial1.print("State:");
  Serial1.println(currentState);

  updateMovement();

  if (millis() - dernierMessagePi >= timeoutPi && currentState == MOVING) {
      currentState = IDLE;
  }

  delay(50);
}