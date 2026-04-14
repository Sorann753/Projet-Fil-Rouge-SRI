#ifndef BLUETOOTH_HPP
#define BLUETOOTH_HPP

#include <Arduino.h>

/**
 * @brief Initialise le port Serial1 à 230400 bauds
 */
void initBluetooth();

/**
 * @brief Transfère les données entre Serial (USB) et Serial1 (BT)
 */
void updateBluetoothBridge();

#endif