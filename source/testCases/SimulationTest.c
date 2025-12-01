/**
 * @author GHOUILEM Abdelhafidh
 */

#include <stdio.h>
#include <stdlib.h> /*pour utiliser system()*/
#include "testCases/SimulationTest.h"
#include "actuator/SimulatorController.h"
#include "utils/position.h"


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

    /*lancer la simulation*/
    system("python3 source/actuator/Simulator.py\n");

    printf("----- FIN TEST SIMULATION -----\n");
}