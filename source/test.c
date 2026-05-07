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
#include "testCases/clusterTest.h"
#include "testCases/arduinoTest.h"
#include "testCases/testNPLprocess.h"

int main(void)
{
    // history_init();
    // homeMenu();
    clusterTest();
    arduinoTest();

    printf("\n----------------------------------------------------------\n");

    // history_close();
    testNPLprocess();
    return 0;
}
