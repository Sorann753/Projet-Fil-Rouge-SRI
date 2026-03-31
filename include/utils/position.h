/**
 *  @author GHOUILEM Abdelhafidh
 */
#ifndef __POSITION_H__
#define __POSITION_H__

#include <math.h>

/*definir la postion du robot*/
typedef struct RobotPostion
{
    float x;
    float y;
    float theta;

} RobotPosition;

typedef struct IntCoordinate_t {
    long x;
    long y;
} IntCoordinate;

typedef struct PolarCoordinate_t {
    float dist;
    float theta;
} PolarCoordinate;

/*---- CONSTANTES ----*/
#ifndef PI
#define PI 3.1415926535897932
#endif

/*---- PROTOTYPES ----*/

/**
 * @brief convertie du degrée au radiant
 */
float degr_to_rad(float angle_deg);

/**
 * @brief convertie du radiant au degrée
 */
float rad_to_degr(float angle_rad);

/**
 * @brief find the euclidean distance between two points
 */
double euclidDist(IntCoordinate P1, IntCoordinate P2);

#endif