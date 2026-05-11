#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/select.h>

#include "rbiArduinoInterface/arduino_interface.h"

int main(void) {
    char* port = "/dev/ttyACM0";
    char input[64];
    char cmd[32];
    int valeur;
    RobotData telemetry;

    if (initSerial(port) != 0) {
        fprintf(stderr, "[ERREUR] Impossible d'ouvrir %s\n", port);
        return 1;
    }

    printf("--- CONTROLEUR UPSSIBOT (TEMPS REEL) ---\n");
    printf("[LOG] Attente reveil Arduino (2s)...\n");
    sleep(2);
    printf("[LOG] ENTREZ COMMANDE...\n");
    while (true) {
        char* msg = readArduino();
        if (msg != NULL && parseTelemetry(msg, &telemetry)) {
            printf("\r[TELE] Etat: %d | Action: %s | Dist: %d cm          ", 
                    telemetry.state, telemetry.cmd, telemetry.distAv);
            fflush(stdout);
        }

        fd_set readfds;
        struct timeval tv;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);

        tv.tv_sec = 0;
        tv.tv_usec = 10000;

        if (select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv) > 0) {
            if (fgets(input, sizeof(input), stdin)) {
                if (sscanf(input, "%s %d", cmd, &valeur) >= 1) {
                    if (strcmp(cmd, "QUIT") == 0) break;
                    
                    if (strcmp(cmd, "STOP") == 0) sendCommand("STOP", 0);
                    else sendCommand(cmd, valeur);
                    
                    printf("\n[LOG] Envoi : %s %d\n", cmd, valeur);
                }
            }
        }
        
        usleep(1000);
    }

    printf("\n[LOG] Fermeture.\n");
    return 0;
}