/**
 * @author GHOUILEM Abdelhafidh
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "actuator/SimulatorController.h"
#include "configLoader/configLoader.h"
#include "history/history.h"
#include "utils/position.h"
#include <assert.h>
#include <stdbool.h>
#include <math.h>

/**
 * TODO: lecture du json (config) pour avoir la position initial du robot
 * TODO: normalisation de langle dans trun() pour quil ne sort pas de lintervalle
 * TODO: limiter la fenétre de simulation pour que le robot ne sort pas
 * TODO: creation et lecture du .map
 * TODO: dessiner le .map dans la simulation
 * TODO: ajouter linteraction avec les obstacles
 * TODO: ecriture dans historique
 * */

/*chemin du fichier de sortie*/
#define SIM_FILE "./SimulatorController.txt"
const char * python_path = NULL;
/**
 * @brief ouverture et lecture de la position initial (config)
 */
void read_sim_config(RobotPosition *Position)
{
    char *x_str = config_loader("config/simulatorConfig.toml", "initial_x");
    char *y_str = config_loader("config/simulatorConfig.toml", "initial_y");

    if (!x_str || !y_str)
    {
        fprintf(stderr, "ERROR: cannot read robot initial positions from TOML\n");
        free(x_str);
        free(y_str);
        return;
    }

    Position->x = atof(x_str);
    Position->y = atof(y_str);
    Position->theta = 0.0f;

    free(x_str);
    free(y_str);

    //printf("Robot initial position: x=%.2f, y=%.2f\n", Position->x, Position->y);

    /* Initialisation du fichier de simulation */
    FILE *action_file = fopen(SIM_FILE, "w");
    if (!action_file)
    {
        fprintf(stderr, "ERROR: could not open %s\n", SIM_FILE);
        return;
    }

    fprintf(action_file, "INIT %f %f\n", Position->x, Position->y);
    fclose(action_file);
}

/**
 * @brief initialiser SimulatorContorller.txt et lecture de la config
 */
void init_Simulator(RobotPosition *Position)
{
    /*ouvrire le fichier simulatorcontroller.txtt */
    python_path = config_loader("config/globalConfig.toml", "python_simulation_path");
    char full_simu_path[512];
    snprintf(full_simu_path, sizeof(full_simu_path), "../../../../%s & 2>/dev/null", python_path);

    char commande[600];
    snprintf(commande, sizeof(commande), "python3 %s &", full_simu_path);

    system(commande);
    
    FILE *action_file = fopen(SIM_FILE, "w");
    if (action_file == NULL)
    {
        fprintf(stderr, "\nSIM_FILE: the SimulationController hase note been initalisated\n\n");
        return;
    }

    /*reset de la position*/
    Position->x = 0.0f;
    Position->y = 0.0f;
    Position->theta = 0.0f;

    fclose(action_file);
    //printf("\nle fichier SimulatorContoller.txt a été vidé !\n");
    
    read_sim_config(Position);
}

/**
 * @brief procédure pour ecrire dans un fichier laction et la valeur dans SimulatorController.txt
 * @param act laction enumerer dans le .h
 * @param value la valuer associer a laction
 */


void closeSimu()
{
    FILE *action_file = fopen(SIM_FILE, "a");

    fprintf(action_file, "CLOSE 0\n");
    fflush(action_file);
    
    fclose(action_file);
}

void WriteAction(action_t act, float value)
{ /*le char (pointeur = string) est en const (appliquer sur le char on ne le modifie pas dans la fonction) */

    /*ouvrire le fichier (mode a = ecrire a la fin du fichier)*/
    FILE *action_file = fopen(SIM_FILE, "a");

    /*test de louverture du fichier*/
    if (action_file == NULL)
    {
        printf("\n\nERREUR (SIM_FILE): the SimulationControler.txt hase note been able to open\n\n");
        return;
    }

    /*utilisation dun switch case pour ecrire en fonction des actions mise en entrer*/
    switch (act)
    {
    case ACT_FORWARD:
        fprintf(action_file, "FORWARD %f\n", value);
        break;

    case ACT_BACKWARD:
        fprintf(action_file, "BACKWARD %f\n", value);
        break;

    case ACT_TURN:
        fprintf(action_file, "TURN %f\n", value);
        break;

    default:
        fprintf(action_file, "ACTION %f\n", value);
        break;
    }

    fclose(action_file);
}

/**
 * @brief procédure pour avancer d'une distance donner en entrer dans en simulation
 * @param distance (en metre)
 */
void forward(float distance, RobotPosition *Position)
{
    if (distance < 0)
    {
        backward(distance, Position);
        return;
    }

    /*conversion de theta en radiant (car le cos et sin utilisent des radiant)*/
    float theta_rad = degr_to_rad(Position->theta);

    /*mise a jour de la position*/
    Position->x = Position->x + distance * cosf(theta_rad);
    Position->y = Position->y + distance * sinf(theta_rad);

    /*apelle de la fonction pour ecrire dans le SimulatorControler.txt pour pouvoir communiquer avec le python*/
    WriteAction(ACT_FORWARD, distance);
    printf("FORWARD: position(%.0f,%.0f) \n", Position->x, Position->y);
}

/**
 * @brief procédure qui utilise forward() en changent le signe
 * @param distance
 */
void backward(float distance, RobotPosition *Position)
{
    /*on prend la valeur absolue*/
    distance = fabsf(distance);

    /*conversion de theta en radiant*/
    float theta_rad = degr_to_rad(Position->theta);

    /*mise a jour de la position*/
    Position->x = Position->x - distance * cosf(theta_rad);
    Position->y = Position->y - distance * sinf(theta_rad);

    /*apelle de la fonction pour ecrire dans le SimulatorControler.txt pour pouvoir communiquer avec le python*/
    WriteAction(ACT_BACKWARD, distance);
    printf("BACKWARD: position(%.0f,%.0f) \n", Position->x, Position->y);
}

/**
 * @brief procédure pour tourner d'un angle en simulation
 * @param angle (en degrer entre -360 et 360)
 */
void turn(float angle, RobotPosition *Position)
{
    if (angle > 360 || angle < (-360))
    {
        fprintf(stderr, "turn(): angle value must be between -360 and +360 degr \n");
        return;
    }

    /*TODO: Mise a jour du theta*/
    Position->theta += angle;

    /*apelle de la fonction pour ecrire dans le SimulatorControler.txt pour pouvoir communiquer avec le python*/
    WriteAction(ACT_TURN, angle);
    printf("TURN: theta= %.0f\n", Position->theta);
}
