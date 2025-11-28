/**
 * @brief dans ce fichier on exécutute tout les cas de test qui sont listé dans source/testCases/
 */

#include "testCases/matrixTest.h"
#include "testCases/testMenu.h"
#include "testCases/SimulationTest.h"

int main(void){

    testDoubleMatrix();

    testByteMatrix();
    
    TestAction();
    
    // keep this one as the last since it require user input
    testMenu();

    /*lancer la simulation*/
    system("python3 source/actuator/Simulator.py");
    return 0;
}