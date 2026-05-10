#include "cartography/cartography.h"


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


Coordinate getCenter(Cluster obj){
    float sumX = 0;
    float sumY = 0;

    for(int i = 0; i < obj.pointCount; i++){
        sumX += obj.cartesianPoints[i].x;
        sumY += obj.cartesianPoints[i].y;
    }

    Coordinate centerPoint = {
        .x = sumX / obj.pointCount,
        .y = sumY / obj.pointCount,
    };

    return centerPoint;
}

/**
 * @brief get the curvature angle at B
 * @return the signed angle curved such that
 * 0 if straight line,
 * pi if turn left
 * -pi if turn right
 * and all the in-between
 */
double getCurvature(Coordinate A, Coordinate B, Coordinate C){
    // compute the movement vectors
    double Ux = B.x - A.x;
    double Uy = B.y - A.y;

    double Vx = C.x - B.x;
    double Vy = C.y - B.y;

    double dotProduct = (Ux*Vx) + (Uy*Vy);
    double crossProduct = (Ux*Vx) - (Uy*Vy);

    return atan2(crossProduct, dotProduct);
}

ShapeKind recognizeShape(Cluster obj){
    const double sharpThreshold = degr_to_rad(30);
    const double StraightThreshold = degr_to_rad(5);
    const double CurvedThreshold = degr_to_rad(30);

    // compute local curvature
    // and compute global curvature
    double curvatures[10]; //TODO
    double globalCurvature = 0;
    double sharpTurnCount = 0;

    for(int i = 1; i < (obj.pointCount-1); i++){
        curvatures[i] = getCurvature(obj.cartesianPoints[i-1], obj.cartesianPoints[i], obj.cartesianPoints[i+1]);
        globalCurvature += curvatures[i];

        // count the sharp turns
        if(curvatures[i] > (-sharpThreshold) && curvatures[i] < (sharpThreshold)){
            sharpTurnCount++;
        }
    }

    if(sharpTurnCount == 0){
        // if very small global curvature -> return Line
        if(globalCurvature > (-StraightThreshold) && globalCurvature < (StraightThreshold)){
            // we assume there is no sinus like shapes so this is enough to conclude
            return LINE_SHAPE;
        }

        // if high global curvature && sharp turn == 0 -> return round shape
        if(globalCurvature < (-CurvedThreshold) || globalCurvature > CurvedThreshold){
            return ROUND_SHAPE;
        }
    }
    else if(sharpTurnCount == 1){
        return ANGLE_SHAPE;
    }

    // else return Other shape unknown polygon
    return OTHER_SHAPE;
}

Shape fitLine(Cluster obj){
    //TODO
}

Shape fitRect(Cluster obj){
    //TODO
}

Shape fitCirle(Cluster obj){
    //TODO
}

Shape makeOtherShape(Cluster obj){
    return (Shape){
        .kind = OTHER_SHAPE,
        .raw = obj,
    };
}

Shape* makeShapes(Cluster* clusters, size_t count){
    Shape fittedShapes[count];
    for(int i = 0; i < count; i++){
        ShapeKind kind = recognizeShape(clusters[i]);
        switch (kind){
            case LINE_SHAPE:
                fittedShapes[i] = fitLine(clusters[i]);
            break;

            case ROUND_SHAPE:
                fittedShapes[i] = fitCirle(clusters[i]);
            break;

            case ANGLE_SHAPE:
                fittedShapes[i] = fitRect(clusters[i]);
            break;
            
            default:
                fittedShapes[i] = makeOtherShape(clusters[i]);
            break;
        }
    }
}
