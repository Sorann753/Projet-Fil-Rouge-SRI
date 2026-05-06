#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include "rbiArduinoInterface/arduino_interface.h"

int main(void) {
    printf("--- DEMARRAGE DU CONTROLEUR ROBOT ---\n");

    char* port = "/dev/ttyACM0";
    if (initSerial(port) != 0) {
        fprintf(stderr, "[ERREUR] Impossible d'ouvrir le port %s\n", port);
        return 1;
    }

    printf("[LOG] Connexion reussie. Attente du reveil de l'Arduino (2s)...\n");
    sleep(2);

    RobotData telemetry;
    char cmd[32];
    int valeur;

    printf("\nCommandes disponibles : FORWARD, BACKWARD, LEFT, RIGHT, STOP, QUIT\n");
    printf("Format : <COMMANDE> <VALEUR> (ex: FORWARD 100)\n");

    while (true) {
        char* msg = readArduino();
        if (msg != NULL) {
            if (parseTelemetry(msg, &telemetry)) {
                printf("\r[TELE] Etat: %d | Action: %s | Distances -> Av:%d G:%d D:%d       ", 
                        telemetry.state, telemetry.cmd, telemetry.distAv, telemetry.distG, telemetry.distD);
                fflush(stdout);
            }
        }

        printf("\nSaisir commande : ");
        if (scanf("%s", cmd) == 1) {
            
            if (strcmp(cmd, "QUIT") == 0) {
                break;
            }

            if (strcmp(cmd, "STOP") != 0) {
                if (scanf("%d", &valeur) == 1) {
                    sendCommand(cmd, valeur);
                    printf("[PI -> ARDUINO] %s : %d envoye.\n", cmd, valeur);
                }
            } else {
                sendCommand("STOP", 0);
                printf("[PI -> ARDUINO] STOP envoye.\n");
            }
        }

        sleep(1); 
    }

    printf("\n[LOG] Fermeture du programme et fin des tests.\n");
    return 0;
}
















/*#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

//#define SIMULATOR
#define HARDWARE

#include "utils/matrix_impl.h"
#include "controlMod/IControlMod.h"
#include "actuator/IActuator.h"

#ifdef SIMULATOR
#include "history/history.h"
#include "controlMod/menu.h"
#endif

#ifdef HARDWARE
#endif



int main(void){
    #if defined (SIMULATOR) && defined (HARDWARE)
    #error you are not supposed to enable both at once
    #endif

    IActuator actuator;
    IControlMod controller;

    #ifdef SIMULATOR
    actuator = makeSimulActuator();
    controller = makeSimulController();
    #endif

    #ifdef HARDWARE
    actuator = makeHardActuator();
    controller = makeHardController();
    #endif

    if(actuator.init != NULL){
        actuator.init(NULL);
    }
    if(controller.init != NULL){
        controller.init();
    }

    return 0;
    
}
*/