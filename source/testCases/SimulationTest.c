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
void TestAction(void){
    printf("----- TEST SIMULATION -----\n");
    
    /*initialisation du fichier SimulatorController.txt*/
    init_Simulator();

    /*TEST: fonction de deplacement*/
    backward(100);
    forward(300);
    turn(45);
    forward(120);
    turn(-90);
    forward(400);
    backward(550);
    turn(50);
    forward(600);


    /*TEST: getter_position*/
    RobotPosition my_robot_position = getter_position();
    printf("la position de mon robot : (%.2f, %.2f) anlge: %.3f \n",my_robot_position.x, my_robot_position.y, my_robot_position.theta);

    /*definir la commande a executer dans le terminale*/
    char commande[600];
    /*ecrire dans commande*/
    sprintf(commande, "python3 %s", SIMULATION_PATH);


    /*lancer la simulation*/
    system(commande);

    printf("----- FIN TEST SIMULATION -----\n");
}