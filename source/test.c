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

int main(void)
{

    test_cutter();

    testDoubleMatrix();

    testByteMatrix();

    testTreeMap();

    testTreeManyInsert(100000);

    testHeaderParsing();

    TestAction();

    testLoadConfig();

    testHistory(INFO, "Ecriture sur l'historique");

    // keep this one as the last since it require user input
    testMenu();

    return 0;
}