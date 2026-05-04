#include <stdio.h>
#include "testCases/testLidar.h"
#include "cartography/lidar.h"

void testLidarAcquisition(void) {
    if (lidar_start() != 0) {
        printf("Erreur : Impossible de démarrer le Lidar.\n");
        return;
    }

    lidar_reset_buffer();

    printf("Acquisition d'un scan...\n");
    lidar_update_scan();

    printf("\n--- AFFICHAGE DES POINTS ACQUIS (%d points) ---\n", current_total);
    printf("Index | Angle (°) | Distance (mm)\n");
    printf("------------------------------------\n");

    for (int i = 0; i < current_total; i++) {
        printf("[%d] | %.2f° | %.2f mm\n", 
               i, 
               lidar_data_buffer[i].theta, 
               lidar_data_buffer[i].dist);
    }

    printf("------------------------------------\n");

    lidar_close();
}