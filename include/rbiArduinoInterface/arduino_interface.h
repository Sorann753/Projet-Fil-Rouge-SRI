#ifndef ARDUINO_INTERFACE_H
#define ARDUINO_INTERFACE_H

typedef struct {
    int state;
    char cmd[32];
    int duration;
    int distAv;
    int distG;
    int distD;
} RobotData;

int initSerial(const char* port);
void sendCommand(const char* action, int value);
char* readArduino();
int parseTelemetry(char* line, RobotData* data);

#endif