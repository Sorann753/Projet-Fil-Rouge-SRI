#include <stdio.h>
#include "cartography/cartography.h"
#include "utils/position.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cartography/cartography.h"
#include "utils/position.h"

Cluster* makeClusters(PolarCoordinate* rawPoints, size_t pointCount, size_t* clusterCount)
{
    if (pointCount == 0 || rawPoints == NULL) return NULL;

    Coordinate* rawPointsCartesian = malloc(pointCount * sizeof(Coordinate));
    if (rawPointsCartesian == NULL) return NULL;
    printf("angle - dist");
    for (size_t i = 0; i < pointCount; i++) {
        rawPointsCartesian[i] = PolarToCartesian(rawPoints[i]);
    }

    Cluster* clusterList = malloc(pointCount * sizeof(Cluster));
    if (clusterList == NULL) {
        free(rawPointsCartesian);
        return NULL;
    }

    *clusterCount = 1;
    size_t currentIdx = 0;
    int distance_max = 200;

    clusterList[0].containedPoints = malloc(pointCount * sizeof(PolarCoordinate));
    clusterList[0].containedPoints[0] = rawPoints[0];
    clusterList[0].pointCount = 1;

    //association 
    for (size_t i = 1; i < pointCount; i++) {
        double dx = rawPointsCartesian[i].x - rawPointsCartesian[i-1].x;
        double dy = rawPointsCartesian[i].y - rawPointsCartesian[i-1].y;
        double dist = sqrt(dx*dx + dy*dy);

        if (dist > distance_max) {
            currentIdx++;
            clusterList[currentIdx].containedPoints = malloc((pointCount - i) * sizeof(PolarCoordinate));
            clusterList[currentIdx].pointCount = 0;
            (*clusterCount)++;
        }

        size_t pIdx = clusterList[currentIdx].pointCount;
        clusterList[currentIdx].containedPoints[pIdx] = rawPoints[i];
        clusterList[currentIdx].pointCount++;
    }

    free(rawPointsCartesian);
    return clusterList;
}