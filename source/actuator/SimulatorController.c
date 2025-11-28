#include <stdio.h>
#include <stdlib.h>
#include "actuator/SimulatorController.h"
#include <assert.h>
#include <stdbool.h>
#include <math.h>

/*chemin du fichier de sortie*/
#define SIM_FILE "./SimulatorController.txt"

/**
 * @brief procédure pour efacer le contenue de SimulatorContorller.txt
 */
void init_Simulator(void){
    FILE* my_file= fopen(SIM_FILE, "w");
    if (my_file == NULL){
        printf("SIM_FILE: the SimulationController hase note been initalisated");
        return;
    }

    printf("le fichier SimulatorContoller.txt a été vidé !");

}



/**
 * @brief procédure pour ecrire dans un fichier laction et la valeur dans SimulatorController.txt
 * @param act laction enumerer dans le .h
 * @param value la valuer associer a laction 
 */
void WriteAction(action act, float value){ /*le char (pointeur = string) est en const (appliquer sur le char on ne le modifie pas dans la fonction) */
    
    /*ouvrire le fichier (mode a = ecrire a la fin du fichier)*/
    FILE* my_file = fopen(SIM_FILE, "a" );
    
    /*test de louverture du fichier*/
    if (my_file==NULL){
        printf("SIM_FILE: the SimulationControler.txt hase note been able to open");
        return;
    }


    /*utilisation dun switch case pour ecrire en fonction des actions mise en entrer*/
    switch (act)
    {
    case ACT_FORWARD:
        fprintf(my_file, "FORWARD %f\n", value);
        printf("ligne ajouter a SimulatorController.txt");
        break;

    case ACT_BACKWARD:
        fprintf(my_file, "BACKWARD %f\n", value);
        printf("ligne ajouter a SimulatorController.txt");
        break;

    case ACT_TURN:
        fprintf(my_file, "TURN %f\n", value);
        printf("ligne ajouter a SimulatorController.txt");
        break;

    default:
        fprintf(my_file, "ACTION %f\n", value);
        printf("(default) ligne ajouter a SimulatorController.txt");
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
        backward(distance);
    }
    
    /*apelle de la fonction pour ecrire dans le SimulatorControler.txt pour pouvoir communiquer avec le python*/
    WriteAction(ACT_FORWARD, distance);
    printf("l'action forward a été ajouter \n");
} 



/**
 * @brief procédure qui utilise forward() en changent le signe 
 * @param distance
 */
void backward(float distance){
    
    distance = fabsf(distance);


    /*apelle de la fonction pour ecrire dans le SimulatorControler.txt pour pouvoir communiquer avec le python*/
    WriteAction(ACT_BACKWARD, distance);
    printf("l'action backward a été ajoute \n");
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
    printf("l'action turn a été ajouter\n");
}





