/**
 * @author GHOUILEM_Abdelhafidh
 */
#ifndef SIMULATOR_CONTROLLER_H
#define SIMULATOR_CONTROLLER_H
#include <stdint.h> /*pour ajouter les type a taille garantie*/

/*.h commun a la simulation et au hardware*/


/*-------------- SIMULATION ---------------*/

/*creation de type*/
typedef enum action_t{
    ACT_FORWARD,
    ACT_BACKWARD,
    ACT_TURN,
}action;


/*TODO: Creer des fonction de deplacement*/


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