/**
 * @brief dans ce fichier on exécutute tout les cas de test qui sont listé dans source/testCases/
 */

#include "history/history.h"

#include "testCases/matrixTest.h"
#include "testCases/testMenu.h"
#include "testCases/SimulationTest.h"
#include "testCases/configLoadTest.h"
#include "testCases/historyTest.h"

int main(void){

    testDoubleMatrix();

    testByteMatrix();
    
    TestAction();
    
    testLoadConfig();

    testHistory(INFO,"Ecriture sur l'historique");

    // keep this one as the last since it require user input
    testMenu();
    
    return 0;
}