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
    sendCommand("FORWARD", 1000);
    
    for(int i = 0; i < 50; i++) { 
        char* msg = readArduino();
        if (msg != NULL && parseTelemetry(msg, &data)) {
            printf("[DATA] Etat: %d | Action: %s | Temps restant: %d ms | Av: %d\n", 
                    data.state, data.cmd, data.duration, data.distAv);
        }
    }

    printf("\n--- TEST 2 : BACKWARD ---\n");
    sendCommand("BACKWARD", 100);
    
    for(int i = 0; i < 50; i++) {
        char* msg = readArduino();
        if (msg != NULL && parseTelemetry(msg, &data)) {
            printf("[DATA] Etat: %d | Action: %s | Distances -> Av:%d\n", 
                    data.state, data.cmd, data.distAv);
        }
    }
    sleep(2);
    printf("\n[LOG] Fin du test.\n");
}