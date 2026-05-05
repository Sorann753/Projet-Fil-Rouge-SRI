/**
 * @author GHOUILEM Abdelhafidh
 */
#include <stdio.h>
#include "utils/position.h"



/**
 * @brief convertie du degrée au radiant
 */
float degr_to_rad(float angle_deg){
    return (angle_deg * (PI / 180.0f));

}

/**
 * @brief convertie du radiant au degrée
 */
float rad_to_degr(float angle_rad){
    return (angle_rad * (180.0f/PI));
}

double euclidDist(IntCoordinate P1, IntCoordinate P2){
    long deltaX = P2.x - P1.x;
    long deltaY = P2.y - P1.y;

    return sqrt(deltaX*deltaX + deltaY*deltaY);
}

Coordinate PolarToCartesian(PolarCoordinate polarCoordinate)
{
    Coordinate coordinate;
    coordinate.x = polarCoordinate.dist*cos(degr_to_rad(polarCoordinate.theta));
    coordinate.y = polarCoordinate.dist*sin(degr_to_rad(polarCoordinate.theta));
    return coordinate;
}