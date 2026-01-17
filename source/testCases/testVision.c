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