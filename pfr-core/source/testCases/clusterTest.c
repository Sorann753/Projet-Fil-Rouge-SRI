#include <stdio.h>
#include "testCases/clusterTest.h"
#include "cartography/lidar.h"
#include "cartography/cartography.h"


void clusterTest(void)
{
    PolarCoordinate* rawPoints; 
    size_t pointCountInt = 0; 
    size_t clusterCount = 0; 

    if (lidar_start() != 0) {
        printf("Erreur : Impossible de démarrer le Lidar.\n");
        return;
    }

    rawPoints = lidar_update_scan(&pointCountInt);
    
    size_t pointCount = (size_t)pointCountInt;

    printf("\n-------- CLUSTER TEST -----------\n");

    Cluster* clusters = makeClusters(rawPoints, pointCount, &clusterCount);

    if (clusters == NULL || clusterCount == 0) {
        printf("Aucun cluster détecté.\n");
    } else {
        printf("Nombre de clusters trouvés : %zu\n", clusterCount);

        for (size_t i = 0; i < clusterCount; i++) {
            printf("\n-- Cluster numéro %zu --\n", i+1);
            printf("Points contenus : %zu\n", clusters[i].pointCount);
            
            for (size_t j = 0; j < clusters[i].pointCount; j++) {
                printf("[%zu] | x : %.2f | y : %.2f \n | ", j, clusters[i].containedPoints[j].theta, clusters[i].containedPoints[j].dist);
            }
        }
    }
    printf("\n-------------------------------\n");
    lidar_close();
}