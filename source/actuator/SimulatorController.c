#include <stdio.h>
#include "actuator/SimulatorController.h"
#include <assert.h>
#include <stdbool.h>

/*chemin du fichier de sortie*/
#define SIM_FILE "./SimulatorController.txt"

/**
 * @brief procédure pour ecrire dans un fichier laction et la valeur
 * @param act laction enumerer dans le .h
 * @param value la valuer associer a laction 
 */
void WriteAction(action act, float value){ /*le char (pointeur = string) est en const (appliquer sur le char on ne le modifie pas dans la fonction) */
    
    /*ouvrire le fichier (mode a = ecrire a la fin du fichier)*/
    FILE* my_file = fopen(SIM_FILE, "a" );
    
    /*test de louverture du fichier*/
    if (!my_file){
        printf("SIM_FILE: the SimulationControler.txt hase note been able to open");
    }


    /*utilisation dun switch case pour ecrire en fonction des actions mise en entrer*/
    switch (act)
    {
    case ACT_FORWARD:
        fprintf(my_file, "FORWARD %f\n", value);
        break;

    case ACT_BACKWARD:
        fprintf(my_file, "BACKWARD %f\n", value);
        break;

    case ACT_TURN:
        fprintf(my_file, "TURN %f\n", value);
        break;

    default:
        fprintf(my_file, "ACTION %f\n", value);
        break;
    }


    fclose(my_file);

}



/**
 * @brief procédure pour avancer d'une distance donner en entrer dans en simulation
 * @param distance (en metre)
 */
void forward(float distance){
    if (distance<0){
        fprintf(stderr, "forward(): distance must be >0 \n");
        return;
    }
    assert(distance<0);

    /*apelle de la fonction pour ecrire dans le SimulatorControler.txt pour pouvoir communiquer avec le python*/
    WriteAction(ACT_FORWARD, distance);
} 



/**
 * @brief procédure qui utilise forward() en changent le signe 
 * @param distance
 */
void backward(float distance){
    if (distance<0){
        fprintf(stderr, "backward(): distance must be >0 \n");
        return;
    }
    assert(distance<0);

    /*apelle de la fonction pour ecrire dans le SimulatorControler.txt pour pouvoir communiquer avec le python*/
    WriteAction(ACT_BACKWARD, distance);

}



/**
 * @brief procédure pour tourner d'un angle en simulation
 * @param angle (en degrer entre -360 et 360)
 */
void turn(float angle){
    if(angle>360 || angle<(-360)){
        fprintf(stderr, "turn(): angle value must be between -360 and +360 degr \n");
        return;
    }
    /*apelle de la fonction pour ecrire dans le SimulatorControler.txt pour pouvoir communiquer avec le python*/
    WriteAction(ACT_TURN, angle);

}





