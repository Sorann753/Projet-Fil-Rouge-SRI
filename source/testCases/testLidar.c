#include "testCases/lidarTest.h"

void testLidarAcquisition(void) {
    printf("----- TEST LIDAR ACQUISITION -----\n");

    LidarBuffer data = get_lidar_scan();

    if (data.count == 0) {
        printf("Erreur : Aucun point récupéré (vérifier config/capteur).\n");
        assert(data.count > 0);
    }

    printf("Points reçus : %d\n", data.count);

    for (int i = 0; i < data.count; i++) {
        assert(data.points[i].angle >= 0.0 && data.points[i].angle <= 360.0);
        assert(data.points[i].distance >= 0.0);
    }

    if (data.count > 0) {
        printf("Premier point - Angle: %.2f, Distance: %.2f mm\n", 
                data.points[0].angle, data.points[0].distance);
    }

    printf("----- TEST SUCCESS -----\n\n");
}