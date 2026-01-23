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
const char *python_path = NULL;

/**
 * @brief fonction pour limiter une valeur entre min et max
 */
static float clamp(float value, float min, float max)
{
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}

/**
 * @brief ouverture et lecture de la position initial (config)
 */
void read_sim_config(RobotPosition *Position)
{
    char *x_str = config_loader("config/simulatorConfig.toml", "initial_x");
    char *y_str = config_loader("config/simulatorConfig.toml", "initial_y");

    if (!x_str || !y_str)
    {
        fprintf(stderr, "SimulatorController.c : ERROR cannot read INITIAL position from TOML\n");
        free(x_str);
        free(y_str);
        return;
    }

    Position->x = atof(x_str);
    Position->y = atof(y_str);
    Position->theta = 0.0f;

    free(x_str);
    free(y_str);

    // printf("Robot initial position: x=%.2f, y=%.2f\n", Position->x, Position->y);

    /* Initialisation du fichier de simulation */
    FILE *action_file = fopen(SIM_FILE, "w");
    if (!action_file)
    {
        fprintf(stderr, "ERROR: could not open %s\n", SIM_FILE);
        history_log(WARNING, "SimulatorController.c : ERROR SIM_FILE could not open");
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
    // lire la config pour avoir la position initiale
    read_sim_config(Position); // récupère x et y du TOML

    FILE *f = fopen(SIM_FILE, "w"); // écrase l'ancien fichier
    if (!f)
        return;

    fprintf(f, "INIT %f %f\n", Position->x, Position->y);
    fclose(f);

    printf("Robot position reset to initial coordinates (%.2f, %.2f)\n", Position->x, Position->y);
    history_log(INFO, "Robot position reset to initial coordinates");
}

/**
 * @brief procédure pour ecrire dans un fichier laction et la valeur dans SimulatorController.txt
 * @param act laction enumerer dans le .h
 * @param value la valuer associer a laction
 */

void startSimu()
{
    python_path = config_loader("config/globalConfig.toml", "python_simulation_path");
    char full_simu_path[512];
    snprintf(full_simu_path, sizeof(full_simu_path), "../../../../%s & 2>/dev/null", python_path);

    char commande[600];
    snprintf(commande, sizeof(commande), "python3 %s &", full_simu_path);

    system(commande);
}

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
    /*lire width et height depuis le toml de config*/
    char *width = config_loader("config/simulatorConfig.toml", "width");
    char *height = config_loader("config/simulatorConfig.toml", "height");
    if (!width || !height)
    {
        fprintf(stderr, "SimulatorController.c : ERROR cannot read config from TOML\n");
        free(width);
        free(height);
        return;
    }

    /*dans turtle (0,0) est au centre donc les limites sont [-width/2, +width/2]*/
    float half_width = atof(width) / 2.0f;
    float half_height = atof(height) / 2.0f;

    free(width);
    free(height);

    /*si la distance est negatif*/
    if (distance < 0)
    {
        backward(-distance, Position);
        return;
    }

    /*conversion de theta en radiant (car le cos et sin utilisent des radiant)*/
    float theta_rad = degr_to_rad(Position->theta);

    /*calculer la nouvelle position*/
    float new_x = Position->x + distance * cosf(theta_rad);
    float new_y = Position->y + distance * sinf(theta_rad);

    /*si on depasse la dimention de la fentre*/
    if (new_x < -half_width)
        new_x = -half_width;
    if (new_x > half_width)
        new_x = half_width;

    if (new_y < -half_height)
        new_y = -half_height;
    if (new_y > half_height)
        new_y = half_height;

    /*calculer la distance reellement parcourue (pour le simulateur)*/
    float dx = new_x - Position->x;
    float dy = new_y - Position->y;
    float actual_distance = sqrtf(dx * dx + dy * dy);

    /*mise a jour de la position*/
    Position->x = new_x;
    Position->y = new_y;

    /*apelle de la fonction pour ecrire dans le SimulatorControler.txt pour pouvoir communiquer avec le python*/
    WriteAction(ACT_FORWARD, actual_distance);
    printf("FORWARD: position(%.0f,%.0f) \n", Position->x, Position->y);
}

/**
 * @brief procédure qui utilise forward() en changent le signe
 * @param distance
 */
void backward(float distance, RobotPosition *Position)
{
    /*lire width et height depuis le toml de config*/
    char *width = config_loader("config/simulatorConfig.toml", "width");
    char *height = config_loader("config/simulatorConfig.toml", "height");
    if (!width || !height)
    {
        fprintf(stderr, "SimulatorController.c : ERROR cannot read config from TOML\n");
        free(width);
        free(height);
        return;
    }

    /*dans turtle (0,0) est au centre donc les limites sont [-width/2, +width/2]*/
    float half_width = atof(width) / 2.0f;
    float half_height = atof(height) / 2.0f;

    free(width);
    free(height);

    /*on prend la valeur absolue*/
    distance = fabsf(distance);

    /*conversion de theta en radiant*/
    float theta_rad = degr_to_rad(Position->theta);

    /*calculer la nouvelle position (direction inverse)*/
    float new_x = Position->x - distance * cosf(theta_rad);
    float new_y = Position->y - distance * sinf(theta_rad);

    /*si on depasse la dimention de la fentre*/
    if (new_x < -half_width)
        new_x = -half_width;
    if (new_x > half_width)
        new_x = half_width;

    if (new_y < -half_height)
        new_y = -half_height;
    if (new_y > half_height)
        new_y = half_height;

    /*calculer la distance reellement parcourue*/
    float dx = Position->x - new_x;
    float dy = Position->y - new_y;
    float actual_distance = sqrtf(dx * dx + dy * dy);

    /*mise a jour de la position*/
    Position->x = new_x;
    Position->y = new_y;

    /*apelle de la fonction pour ecrire dans le SimulatorControler.txt pour pouvoir communiquer avec le python*/
    WriteAction(ACT_BACKWARD, actual_distance);
    printf("BACKWARD: position(%.0f,%.0f) \n", Position->x, Position->y);
}

/**
 * @brief procédure pour tourner d'un angle en simulation
 * @param angle (en degrer)
 */
void turn(float angle, RobotPosition *Position)
{
    /*Mise a jour du theta*/
    Position->theta += angle;

    /*normalisation de langle pour quil reste entre -360 et 360*/
    while (Position->theta > 360.0f)
        Position->theta -= 360.0f;
    while (Position->theta < -360.0f)
        Position->theta += 360.0f;

    /*apelle de la fonction pour ecrire dans le SimulatorControler.txt pour pouvoir communiquer avec le python*/
    WriteAction(ACT_TURN, angle);
    printf("TURN: theta= %.0f\n", Position->theta);
}