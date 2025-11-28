/**
 * @author GHOUILEM Abdelhafidh
 */

#include <stdio.h>
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
    backward(1);
    forward(30);
    forward(3);
    turn(4);
    forward(7);
    backward(4);
    forward(6);

    printf("----- FIN TEST SIMULATION -----\n");
}