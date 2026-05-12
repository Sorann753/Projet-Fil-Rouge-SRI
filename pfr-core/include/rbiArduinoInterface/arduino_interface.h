#ifndef ARDUINO_INTERFACE_H
#define ARDUINO_INTERFACE_H
#ifdef __cplusplus
extern "C" {
#endif

#define DIST_LIMITE_AVANT 40   // Distance s'arrêter=>tourner
#define DIST_REPRISE_MARCHE 60 // Distance reprise

typedef struct
{
    char cmd[32];
    int state;
    int duration;
    int distAv;
    int distG;
    int distD;
} RobotData;

/*mode de fonctionnement*/
enum MODE
{
    MANUEL,
    AUTONOME
};

int initSerial(const char *port);
void sendCommand(const char *action, int value);
char* readArduino(void);
int parseTelemetry(const char *line, RobotData *data);
void closeArduino(void);

#ifdef __cplusplus
}
#endif
#endif