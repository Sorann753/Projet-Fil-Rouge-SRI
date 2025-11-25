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
    
    /*tester les fonction de deplacement*/
    backward(100);
    forward(55);
    forward(63);
    turn(45);
    forward(47);
    backward(44);
    forward(60);

    printf("----- FIN TEST SIMULATION -----\n");
}