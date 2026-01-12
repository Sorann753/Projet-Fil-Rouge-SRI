#include "testCases/testVision.h"

void testHeaderParsing(void){
    printf("----- TEST HEADER PARSING -----\n");

    const char* testHeader = "100 200 3";

    ImageHeader header = {
        .length = 0,
        .height = 0,
        .canalCount = 0,
    };
    header = parseHeader(testHeader);

    printf("Test header : %s\n", testHeader);

    printf("length : %ld\n", header.length);
    assert(header.length == 100);

    printf("height : %ld\n", header.height);
    assert(header.height == 200);

    printf("canalCount : %d\n", header.canalCount);
    assert(header.canalCount == 3);

    printf("----- TEST SUCCESS -----\n\n");
}

// void testImageLoading(void){
//     printf("----- TEST IMAGE LOADING -----\n");

//     const char* testImagePath = "./data/IMG_5389.txt";
    
//     printf("Testing on image : %s\n", testImagePath);
//     Image img = loadImage(testImagePath);

//     assert(img.isValid);
//     printf("LOADED\n");

//     assert(img.columns == 300);
//     assert(img.lines == 300);

//     // verify the getter
//     Pixel p = getPixel(&img, 0, 0);
//     assert(p.R == 93);
//     assert(p.G == 16);
//     assert(p.B == 10);


//     assert(getPixel(&img, 0, 5).R == 94); // verify that the Y axis is correct
//     assert(getPixel(&img, 5, 0).R == 95); // verify that the X axis is correct


//     //cleanup
//     freeMatrix(&img);

//     printf("----- TEST SUCCESS -----\n\n");
// }