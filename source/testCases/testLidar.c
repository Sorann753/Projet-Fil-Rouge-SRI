#include <stdio.h>
#include "testCases/testLidar.h"
#include "cartography/lidar.h"

void testLidarAcquisition(void) {

    PolarCoordinate* scan_data; 
    size_t point_number;

    if (lidar_start() != 0) {
        printf("Erreur : Impossible de démarrer le Lidar.\n");
        return;
    }

    for (int i =0; i<3;i++)
    {
        point_number = 0;

        printf("Acquisition d'un scan...\n");
        scan_data = lidar_update_scan(&point_number);
    
        printf("\n--- AFFICHAGE DES POINTS ACQUIS (%ld points) ---\n", point_number);
        printf("Index | Angle (°) | Distance (mm)\n");
        printf("------------------------------------\n");

        for (int i = 0; i < point_number; i++) 
        {
            printf("[%d] | %.2f° | %.2f mm\n", i, scan_data[i].theta, scan_data[i].dist);
        }
        printf("------------------------------------\n");
        
    } 
    lidar_close();
}