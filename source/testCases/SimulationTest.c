/**
 * @author GHOUILEM Abdelhafidh
 */

#include <stdio.h>
#include <stdlib.h> /*pour utiliser system()*/
#include "testCases/SimulationTest.h"
#include "configLoader/configLoader.h"
#include "actuator/SimulatorController.h"
#include "utils/position.h"

/**
 * @brief test des fonctions qui ecrivent dans SimulatorController.txt pour communiquer avec le python
 */
void TestAction(void)
{
    printf("----- TEST SIMULATION -----\n");

    char *python_path = config_loader("config/globalConfig.toml", "python_simulation_path");

    /*initialisation de la position du robot*/
    RobotPosition my_robot;

    /*initialisation du fichier SimulatorController.txt*/
    init_Simulator(&my_robot);

    /*TEST: fonction de deplacement*/
    forward(300, &my_robot);
    turn(90, &my_robot);
    forward(100, &my_robot);
    backward(50, &my_robot);

    printf("position: (%.2f, %.2f) anlge: %.3f \n", my_robot.x, my_robot.y, my_robot.theta);

    printf("\nCONTENU DE SimulatorController.txt :\n");
    system("cat ./SimulatorController.txt");
    printf("\n");

    /* creation d'une variable pour trouver le chemin du script python a executer*/
    char full_python_path[512];
    snprintf(full_python_path, sizeof(full_python_path), "../../../../%s", python_path);

    /*definir la commande a executer dans le terminale*/
    char commande[600];

    /*ecrire dans commande*/
    snprintf(commande, sizeof(commande), "python3 %512s", full_python_path);

    /*lancer la simulation*/
    system(commande);

    printf("----- FIN TEST SIMULATION -----\n");
}