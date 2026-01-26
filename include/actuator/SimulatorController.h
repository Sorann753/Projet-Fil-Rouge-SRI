/**
 * @author GHOUILEM_Abdelhafidh
 */
#ifndef SIMULATOR_CONTROLLER_H
#define SIMULATOR_CONTROLLER_H
#include <stdint.h> /*pour ajouter les type a taille garantie*/
#include "utils/position.h"
#include "../manualPilot/command.h"

/**
 * @brief ouverture et lecture de la position initial (config)
 */
void read_sim_config(RobotPosition *Position);

void startSimu(void);

void closeSimu(void);
/**
 * @brief procédure pour efacer le contenue de SimulatorContorller.txt
 */
void init_Simulator(RobotPosition *Position);

/**
 * @brief procédure pour avancer d'une distance donner en entrer dans en simulation
 * @param distance (en metre)
 * @param position (la structure pour modifier le x, y)
 */
void forward(float distance, RobotPosition *Position);

/**
 * @brief procédure qui utilise forward() en changent le signe
 * @param distance
 * @param position (la structure pour modifier le x, y)
 */
void backward(float distance, RobotPosition *Position);

/**
 * @brief procédure pour tourner d'un angle en simulation
 * @param angle (en degrer)
 * @param position (la structure pour modifier le theta)
 */
void turn(float angle, RobotPosition *Position);

#endif