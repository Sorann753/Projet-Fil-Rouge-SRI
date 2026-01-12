/**
 *  @author GHOUILEM Abdelhafidh
 */
#ifndef __POSITION_H__
#define __POSITION_H__

/*definir la postion du robot*/
typedef struct RobotPostion
{
    float x;
    float y;
    float theta;

} RobotPosition;

/*---- CONSTANTES ----*/
#ifndef PI
#define PI 3.14159265358979323846
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

#endif