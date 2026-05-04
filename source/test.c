/**
 * @brief dans ce fichier on exécutute tout les cas de test qui sont listé dans source/testCases/
 */

#include "history/history.h"
#include "testCases/matrixTest.h"
#include "testCases/testMenu.h"
#include "testCases/testVision.h"
#include "testCases/testTreeMap.h"
#include "testCases/SimulationTest.h"
#include "testCases/configLoadTest.h"
#include "testCases/historyTest.h"
#include "testCases/cuttertest.h"
#include "testCases/vocabularytest.h"
#include "testCases/analysistest.h"
#include "testCases/parsertest.h"
#include "testCases/speechTest.h"
#include "testCases/executortest.h"
#include "testCases/testLidar.h"

int main(void)
{
    printf("\n\n\n\n\n\n");
    //history_init();

    //homeMenu();

    testLidarAcquisition();
    
    //history_close();

    return 0;
}
