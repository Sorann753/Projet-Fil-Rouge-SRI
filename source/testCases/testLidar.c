#include "testCases/lidarTest.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cartography/read_lidar.h"

void testLidarAcquisition(void) {
    printf("----- TEST LIDAR ACQUISITION -----\n");

    int count = 0;
    PolarCoordinate *points = get_lidar_scan(&count);

    if (points == NULL || count == 0) {
        printf("Erreur : Aucun point récupéré (vérifier config/capteur ou allocation).\n");
        assert(count > 0);
        assert(points != NULL);
    }

    printf("Points reçus : %d\n", count);

    for (int i = 0; i < count; i++) {
        assert(points[i].theta >= 0.0 && points[i].theta <= 360.0);
        assert(points[i].dist >= 0.0);
    }

    if (count > 0) {
        printf("Premier point - Angle (theta): %.2f, Distance (dist): %.2f mm\n", 
                points[0].theta, points[0].dist);
    }

    free(points);

    printf("----- TEST SUCCESS -----\n\n");
}