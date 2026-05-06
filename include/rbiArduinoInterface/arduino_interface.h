#ifndef ARDUINO_INTERFACE_H
#define ARDUINO_INTERFACE_H

int initSerial(const char* port);
void sendCommand(const char* action, int value);
char* readArduino();

#endif