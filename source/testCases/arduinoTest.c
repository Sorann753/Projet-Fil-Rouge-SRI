#include <stdio.h>
#include <unistd.h>
#include "rbiArduinoInterface/arduino_interface.h"
#include "testCases/arduinoTest.h"

void arduinoTest(void){
    printf("-----Test envoie commande Arduino------");

    if (initSerial("/dev/ttyACM0") != 0) {
        fprintf(stderr, "[ERREUR] Impossible d'ouvrir /dev/ttyACM0\n");
        return ;
    }

    printf("[LOG] Connexion reussie sur %s\n");
    printf("[LOG] Attente du reveil de l'Arduino (2s)...\n");
    sleep(2);

    printf("\n--- TEST 1 : CALIBRATION ---\n");
    printf("[PI -> ARDUINO] Envoi de FORWARD:100\n");
    sendCommand("FORWARD",100);
    readArduino();
    sleep(2);

    printf("\n--- TEST 2 : STOP ---\n");
    printf("[PI -> ARDUINO] Envoi de STOP:0\n");
    sendCommand("STOP",0);
    readArduino();

    printf("\n[LOG] Fin du test.\n");
    return;
    printf("---------------------------------------");
}



