#include "components/bluetooth.hpp"

void initBluetooth() 
{
  Serial1.begin(230400); 
}

void updateBluetoothBridge() 
{
  char c;

  // Lecture USB -> Envoi vers Bluetooth
  if (Serial.available()) 
  {
    c = Serial.read();
    Serial1.print(c);
  }

  // Lecture Bluetooth -> Envoi vers USB
  if (Serial1.available()) 
  {
    c = Serial1.read();
    Serial.print(c);
  }
}