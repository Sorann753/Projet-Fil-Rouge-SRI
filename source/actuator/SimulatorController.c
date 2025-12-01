/**
 * @author GHOUILEM Abdelhafidh
 */
#include <stdio.h>
#include <stdlib.h>
#include "actuator/SimulatorController.h"
#include "utils/position.h"
#include <assert.h>
#include <stdbool.h>
#include <math.h>

/**
 * TODO: normalisation de langle dans trun() pour quil ne sort pas de lintervalle
 * TODO: limiter la fenétre de simulation pour que le robot ne sort pas 
 * TODO: creation et lecture du .map
 * TODO: dessiner le .map dans la simulation
 * TODO: ajouter linteraction avec les obstacles
 * TODO: ecriture dans historique
 * */


/*chemin du fichier de sortie*/
#define SIM_FILE "./SimulatorController.txt"


/*VARIABLE GLOBALE : initialisation de la position*/
static RobotPosition Position = {0.0f, 0.0f, 0.0f};



/*GETTER: pour recuperer la position actuelle du robot*/
RobotPosition getter_position(void){
    return Position;
}



/**
 * @brief procédure pour efacer le contenue de SimulatorContorller.txt
 */
void init_Simulator(void){
    FILE* my_file= fopen(SIM_FILE, "w");
    if (my_file == NULL){
        printf("SIM_FILE: the SimulationController hase note been initalisated\n");
        return;
    }

    /*reset de la position*/
    Position.x = 0.0f;
    Position.y = 0.0f;
    Position.theta = 0.0f;

    printf("le fichier SimulatorContoller.txt a été vidé !\n");

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
        printf("SIM_FILE: the SimulationControler.txt hase note been able to open\n");
        return;
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
        backward(distance);
    }
    
    /*conversion de theta en radiant*/
    float theta_rad = degr_to_rad(Position.theta);

    /*mise a jour de la position*/
    Position.x = Position.x + distance * cosf(theta_rad);
    Position.y = Position.y + distance * sinf(theta_rad);
    

    /*apelle de la fonction pour ecrire dans le SimulatorControler.txt pour pouvoir communiquer avec le python*/
    WriteAction(ACT_FORWARD, distance);
    printf("FORWARD: position(%.0f,%.0f) \n", Position.x, Position.y);
} 



/**
 * @brief procédure qui utilise forward() en changent le signe 
 * @param distance
 */
void backward(float distance){
    
    distance = fabsf(distance);

    
    /*conversion de theta en radiant*/
    float theta_rad = degr_to_rad(Position.theta);
    

    /*mise a jour de la position*/
    Position.x = Position.x - distance * cosf(theta_rad);
    Position.y = Position.y - distance * sinf(theta_rad);
    

    /*apelle de la fonction pour ecrire dans le SimulatorControler.txt pour pouvoir communiquer avec le python*/
    WriteAction(ACT_BACKWARD, distance);
    printf("BACKWARD: position(%.0f,%.0f) \n", Position.x, Position.y);
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

    /*TODO: Mise a jour du theta*/
    Position.theta += angle;
    
    

    /*apelle de la fonction pour ecrire dans le SimulatorControler.txt pour pouvoir communiquer avec le python*/
    WriteAction(ACT_TURN, angle);
    printf("TURN: theta= %.0f\n", Position.theta);
}





