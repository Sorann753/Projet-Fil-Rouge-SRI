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






