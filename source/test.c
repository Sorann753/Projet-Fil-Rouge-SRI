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

int main(void)
{
    test_vocabulary();
    printf("\n\n**********************************************\n\n");

    test_cutter();
    printf("\n\n**********************************************\n\n");

    test_analysis();
    printf("\n\n**********************************************\n\n");

    test_parser();
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

    TestAction();
    printf("\n\n**********************************************\n\n");

    testLoadConfig();
    printf("\n\n**********************************************\n\n");

    testHistory(INFO, "Ecriture sur l'historique");
    printf("\n\n**********************************************\n\n");


    testGetSpeech();
    // keep this one as the last since it require user input
    testMenu();
    printf("\n\n**********************************************\n\n");

    return 0;
}