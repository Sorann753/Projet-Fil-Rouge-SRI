/**
 * @brief dans ce fichier on exécutute tout les cas de test qui sont listé dans source/testCases/
 */

#include "testCases/matrixTest.h"
#include "testCases/testMenu.h"
#include "testCases/testVision.h"

int main(void){

    testDoubleMatrix();

    testByteMatrix();

    testHeaderParsing();
    
    // keep this one as the last since it require user input
    testMenu();
    return 0;
}