/**
 * @author GHOUILEM_Abdelhafidh
 */
#ifndef SIMULATOR_CONTROLLER_H
#define SIMULATOR_CONTROLLER_H
#include <stdint.h> /*pour ajouter les type a taille garantie*/

/*.h commun a la simulation et au hardware*/

/*TODO fonction pour recuperer la fonction actuelle */

/*-------------- SIMULATION ---------------*/

/*definir les differentes actions*/
typedef enum action_t{
    ACT_FORWARD,
    ACT_BACKWARD,
    ACT_TURN,
}action;

/*definir la postion du robot*/
typedef struct RobotPostion{
    float x;
    float y;
    float theta;
}RobotPosition;



/**
 * @brief procédure pour efacer le contenue de SimulatorContorller.txt
 */
void init_Simulator(void);

/**
 * @brief procédure pour avancer d'une distance donner en entrer dans en simulation
 * @param distance (en metre)
 */
void forward(float distance); 




/**
 * @brief procédure qui utilise forward() en changent le signe 
 * @param distance
 */
void backward(float distance);




/**
 * @brief procédure pour tourner d'un angle en simulation
 * @param angle (en degrer)
 */
void turn(float angle);


#endif