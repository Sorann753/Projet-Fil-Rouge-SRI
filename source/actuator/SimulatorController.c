#include <stdio.h>
#include "actuator/SimulatorController.h"
#include <assert.h>
#include <stdbool.h>


/**
 * @brief procédure pour ecrire dans un fichier laction et la valeur
 * @param act laction enumerer
 * @param value la valuer associer a laction 
 */
void WriteAction(action act, float value){ /*le char (pointeur = string) est en const (appliquer sur le char on ne le modifie pas dans la fonction) */
    
    /*ouvrire le fichier (mode a = ecrire a la fin du fichier)*/
    FILE* my_file = fopen("./SimulatorControler.txt", "a" );
    
    /*test de louverture du fichier*/
    if (!my_file){
        printf("ERREUR: the SimulationControler hase note been able to open");
    }

    /*utilisation dun switch case pour ecrire en fonction des actions mise en entrer*/
    switch (act)
    {
    case ACT_FORWARD:
        fprintf(my_file, "FORWARD %f", value);
        break;

    case ACT_BACKWARD:
        fprintf(my_file, "BACKWARD %f", value);
        break;

    case ACT_TURN:
        fprintf(my_file, "TURN %f", value);
        break;

    default:
        fprintf(my_file, "ACTION %f", value);
        break;
    }


    fclose(my_file);

}



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

    /*apelle de la fonction pour ecrire dans le SimulatorControler.txt pour pouvoir communiquer avec le python*/
    WriteAction(ACT_FORWARD, distance);

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





