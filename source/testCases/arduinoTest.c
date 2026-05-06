#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "rbiArduinoInterface/arduino_interface.h"
#include "testCases/arduinoTest.h"

void arduinoTest(void)
{
    printf("\n----- Test envoi commande Arduino -----\n");

    char* port = "/dev/ttyACM0";
    if (initSerial(port) != 0) {
        fprintf(stderr, "[ERREUR] Impossible d'ouvrir %s\n", port);
        return;
    }

    printf("[LOG] Connexion reussie sur %s\n", port);
    printf("[LOG] Attente du reveil de l'Arduino (2s)...\n");
    sleep(2);

    RobotData data;

    printf("\n--- TEST 1 : FORWARD ---\n");
    printf("[PI -> ARDUINO] Envoi de FORWARD:100\n");
    sendCommand("FORWARD", 100);

    for (int i = 0; i < 10; i++) {
        char* msg = readArduino(); 
        if (msg != NULL && parseTelemetry(msg, &data)) {
            printf("[DATA] Etat: %d | Action: %s | Duree: %d | Distances -> Av:%d G:%d D:%d\n",
                data.state, data.cmd, data.duration, data.distAv, data.distG, data.distD);
        }
        
    }
    sleep(1);
    printf("\n--- TEST 2 : STOP ---\n");
    printf("[PI -> ARDUINO] Envoi de STOP:0\n");
    sendCommand("STOP", 0);

    for (int i = 0; i < 10; i++) {
        char* msg = readArduino();
        if (msg != NULL && parseTelemetry(msg, &data)) {
            printf("[DATA] Etat: %d | Action: %s | Duree: %d | Distances -> Av:%d G:%d D:%d\n",
                data.state, data.cmd, data.duration, data.distAv, data.distG, data.distD);
        }
    }

    printf("\n[LOG] Fin du test.\n");
    printf("---------------------------------------\n");
}