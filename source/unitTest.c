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

#include "configLoader/configLoader.h"

int main(void)
{
    printf("\n\n\n\n\n\n");
    history_init();

    // === matrix testing ===
    testDoubleMatrix();
    testByteMatrix();

    // === treemap testing ===
    testTreeMap();
    testTreeManyInsert(100000);

    // === vision testing ===
    initVision();

    testHeaderParsing("100 200 3", 100, 200, 3);
    testHeaderParsing("1000 50 3\n", 1000, 50, 3);
    testHeaderParsing("   500        300    3   \n", 500, 300, 3);

    ImageFingerPrint img1_info = {
        .lines = 300,
        .columns = 300,
        .startPixel = {93, 16, 10},
        .endPixel = {171, 160, 142},
        .Xpixel = {95, 16, 12},
        .Ypixel = {94, 17, 11}
    };
    testImageLoading("./../../../../data/IMG_5389.txt", img1_info);

    ImageFingerPrint img2_info = {
        .lines = 300,
        .columns = 300,
        .startPixel = {118, 110, 97},
        .endPixel = {127, 119, 108},
        .Xpixel = {126, 118, 105},
        .Ypixel = {112, 104, 91}
    };
    testImageLoading("./../../../../data/IMG_5392.txt", img2_info);

    ImageFingerPrint img3_info = {
        .lines = 300,
        .columns = 300,
        .startPixel = {32, 36, 37},
        .endPixel = {146, 139, 129},
        .Xpixel = {33, 37, 40},
        .Ypixel = {29, 34, 40}
    };
    testImageLoading("./../../../../data/IMG_5400.txt", img3_info);

    
    const char* testImg = config_loader("config/visionConfig.toml", "file");

    testMaskExtract(testImg, "./img.bin");
    testObjectDetect(testImg);


    homeMenu();

    history_close();

    return 0;
}