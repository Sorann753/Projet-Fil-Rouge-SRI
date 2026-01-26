#include "testCases/testVision.h"

void testHeaderParsing(const char* testHeader, const size_t expectedLength, const size_t expectedHeight, const uint8_t expectedCanal){
    printf("----- TEST HEADER PARSING -----\n");

    ImageHeader header = {
        .length = 0,
        .height = 0,
        .canalCount = 0,
    };
    header = parseHeader(testHeader);

    printf("Test header : %s\n", testHeader);

    printf("length : %ld\n", header.length);
    assert(header.length == expectedLength);

    printf("height : %ld\n", header.height);
    assert(header.height == expectedHeight);

    printf("canalCount : %d\n", header.canalCount);
    assert(header.canalCount == expectedCanal);

    printf("----- TEST SUCCESS -----\n\n");
}

void testImageLoading(const char* testImagePath, ImageFingerPrint info){
    printf("----- TEST IMAGE LOADING -----\n");

    printf("Testing on image : %s\n", testImagePath);
    Image img = loadImage(testImagePath);

    assert(img.isValid);
    printf("LOADED\n");

    assert(img.columns == info.columns);
    assert(img.lines == info.lines);

    // verify the getter
    Pixel p = getPixel(&img, 0, 0);
    assert(p.R == info.startPixel.R);
    assert(p.G == info.startPixel.G);
    assert(p.B == info.startPixel.B);

    p = getPixel(&img, info.columns-1, info.lines-1);
    assert(p.R == info.endPixel.R);
    assert(p.G == info.endPixel.G);
    assert(p.B == info.endPixel.B);

    // verify that the Y axis is correct
    p = getPixel(&img, 0, 5);
    assert(p.R == info.Ypixel.R); 
    assert(p.G == info.Ypixel.G); 
    assert(p.B == info.Ypixel.B); 

    // verify that the X axis is correct
    p = getPixel(&img, 5, 0);
    assert(p.R == info.Xpixel.R); 
    assert(p.G == info.Xpixel.G); 
    assert(p.B == info.Xpixel.B); 


    // cleanup
    freeMatrix(&img);

    printf("----- TEST SUCCESS -----\n\n");
}

void testMaskExtract(const char* testFilePath, const char* outPath){
    printf("----- TEST MASK EXTRACTION -----\n");

    Image img = loadImage(testFilePath);
    assert(img.isValid);

    const int colorCount = 8;
    ColorReferences ref = {
        .colorCount = colorCount,
        .refPoints = (Color*)malloc(sizeof(Color) * colorCount),
    };
    if(ref.refPoints == NULL){
        freeMatrix(&img);
        exit(EXIT_FAILURE);
    }

    ref.refPoints[0] = (Color){ //RED
        .R = 255,
        .G = 0,
        .B = 0,
    };
    ref.refPoints[1] = (Color){ //GREEN
        .R = 0,
        .G = 255,
        .B = 0,
    };
    ref.refPoints[2] = (Color){ //BLUE
        .R = 0,
        .G = 0,
        .B = 255,
    };
    ref.refPoints[3] = (Color){ //YELLOW
        .R = 255,
        .G = 255,
        .B = 0,
    };
    ref.refPoints[4] = (Color){ //PURPLE
        .R = 255,
        .G = 0,
        .B = 255,
    };
    ref.refPoints[5] = (Color){ //BLACK
        .R = 0,
        .G = 0,
        .B = 0,
    };
    ref.refPoints[6] = (Color){ //WHITE
        .R = 255,
        .G = 255,
        .B = 255,
    };
    ref.refPoints[7] = (Color){ //GRAY
        .R = 64,
        .G = 64,
        .B = 64,
    };

    ColorMasks masks = extractColors(&img, ref);

    for(uint8_t i = 0; i < masks.colorCount; i++){
        const Matrix* M = &(masks.masks[i]);
        M++;
    }

    exportMasks(outPath, masks, ref);

    freeColorMasks(&masks);
    free(ref.refPoints);
    freeMatrix(&img);

    printf("----- TEST SUCCESS -----\n\n");
}

void testObjectDetect(const char* imgPath){
    printf("----- TEST OBJECT DETECTION -----\n");

    Image img = loadImage(imgPath);
    assert(img.isValid);

    const int colorCount = 8;
    ColorReferences ref = {
        .colorCount = colorCount,
        .refPoints = (Color*)malloc(sizeof(Color) * colorCount),
    };
    if(ref.refPoints == NULL){
        freeMatrix(&img);
        exit(EXIT_FAILURE);
    }

    ref.refPoints[0] = (Color){ //RED
        .R = 255,
        .G = 0,
        .B = 0,
    };
    ref.refPoints[1] = (Color){ //GREEN
        .R = 0,
        .G = 255,
        .B = 0,
    };
    ref.refPoints[2] = (Color){ //BLUE
        .R = 0,
        .G = 0,
        .B = 255,
    };
    ref.refPoints[3] = (Color){ //YELLOW
        .R = 255,
        .G = 255,
        .B = 0,
    };
    ref.refPoints[4] = (Color){ //PURPLE
        .R = 255,
        .G = 0,
        .B = 255,
    };
    ref.refPoints[5] = (Color){ //BLACK
        .R = 0,
        .G = 0,
        .B = 0,
    };
    ref.refPoints[6] = (Color){ //WHITE
        .R = 255,
        .G = 255,
        .B = 255,
    };
    ref.refPoints[7] = (Color){ //GRAY
        .R = 64,
        .G = 64,
        .B = 64,
    };

    ColorMasks masks = extractColors(&img, ref);

    BallArray redBall = findSpheres(&(masks.masks[0]));
    BallArray greenBall = findSpheres(&(masks.masks[1]));
    BallArray blueBall = findSpheres(&(masks.masks[2]));
    BallArray yellowBall = findSpheres(&(masks.masks[3]));
    BallArray purpleBall = findSpheres(&(masks.masks[4]));
    BallArray blackBall = findSpheres(&(masks.masks[5]));
    BallArray whiteBall = findSpheres(&(masks.masks[6]));
    BallArray grayBall = findSpheres(&(masks.masks[7]));

    exportBallsToFile("./red.bin", redBall);
    exportBallsToFile("./green.bin", greenBall);
    exportBallsToFile("./blue.bin", blueBall);
    exportBallsToFile("./yellow.bin", yellowBall);
    exportBallsToFile("./purple.bin", purpleBall);
    exportBallsToFile("./black.bin", blackBall);
    exportBallsToFile("./white.bin", whiteBall);
    exportBallsToFile("./gray.bin", grayBall );

    free(redBall.balls);
    free(greenBall.balls);
    free(blueBall.balls);
    free(yellowBall.balls);
    free(purpleBall.balls);
    free(blackBall.balls);
    free(whiteBall.balls);
    free(grayBall.balls);

    freeColorMasks(&masks);
    free(ref.refPoints);
    freeMatrix(&img);

    printf("----- TEST SUCCESS -----\n\n");
}