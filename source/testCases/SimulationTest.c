/**
 * @author GHOUILEM Abdelhafidh
 */

#include <stdio.h>
#include <stdlib.h> /*pour utiliser system()*/
#include "testCases/SimulationTest.h"
#include "actuator/SimulatorController.h"


/**
 * @brief test des fonctions qui ecrivent dans SimulatorController.txt pour communiquer avec le python
 */
void TestAction(void){
    printf("----- TEST SIMULATION -----\n");
    
    /*initialisation du fichier SimulatorController.txt*/
    init_Simulator();

    /*tester les fonction de deplacement*/
    backward(100);
    forward(300);
    turn(45);
    forward(120);
    turn(-90);
    forward(400);
    backward(550);
    turn(50);
    forward(600);

    /*lancer la simulation*/
    system("python3 source/actuator/Simulator.py");

    printf("----- FIN TEST SIMULATION -----\n");
}