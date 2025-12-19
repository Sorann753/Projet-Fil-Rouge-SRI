/**
 * @author GHOUILEM Abdelhafidh
 */

#include <stdio.h>
#include <stdlib.h> /*pour utiliser system()*/
#include "testCases/SimulationTest.h"
#include "actuator/SimulatorController.h"
#include "utils/position.h"

#define SIMULATION_PATH "/home/hafidh/Documents/GitHub/Projet-Fil-Rouge-SRI/source/actuator/Simulator.py"
/**
 * @brief test des fonctions qui ecrivent dans SimulatorController.txt pour communiquer avec le python
 */
void TestAction(void)
{
    printf("----- TEST SIMULATION -----\n");

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

    /*definir la commande a executer dans le terminale*/
    char commande[600];
    /*ecrire dans commande*/
    sprintf(commande, "python3 %s", SIMULATION_PATH);

    /*lancer la simulation*/
    system(commande);

    printf("----- FIN TEST SIMULATION -----\n");
}