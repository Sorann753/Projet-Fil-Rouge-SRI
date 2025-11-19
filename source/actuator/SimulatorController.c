#include <stdio.h>
#include "actuator/SimulatorController.h"
#include <assert.h>
#include <stdbool.h>

/**
 * @brief procédure pour avancer d'une distance donner en entrer dans en simulation
 * @param distance (en metre)
 */
void forward(float distance){
    assert(distance > 0);
    bool valid_value;
    
    /*tester la validiter de la distance*/
    if (distance > 10000){
        valid_value = false;
    }else valid_value = true;

    /**/




} 


/**
 * @brief procédure qui utilise forward() en changent le signe 
 * @param distance
 */
void backward(float distance){



}



/**
 * @brief procédure pour tourner d'un angle en simulation
 * @param angle (en degrer)
 */
void turn(float angle){




}





