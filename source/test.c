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

int main(void)
{
    const char *phrase = "Avance de 100 metres puis tourne a gauche et recule de 30 metres puis tourne de 45 puis go de 200 et tourne de 60 puis va tout de roit de 100 et cherche le cube rouge";
    history_init();

    test_vocabulary();
    printf("\n\n**********************************************\n\n");

    test_executor(phrase);
    printf("\n\n**********************************************\n\n");

    testDoubleMatrix();
    printf("\n\n**********************************************\n\n");

    testByteMatrix();
    printf("\n\n**********************************************\n\n");

    testTreeMap();
    printf("\n\n**********************************************\n\n");

    testTreeManyInsert(100000);
    printf("\n\n**********************************************\n\n");

    testHeaderParsing();
    printf("\n\n**********************************************\n\n");

    testLoadConfig();
    printf("\n\n**********************************************\n\n");

    // keep this one as the last since it require user input
    testMenu();
    printf("\n\n**********************************************\n\n");

    history_close();

    return 0;
}