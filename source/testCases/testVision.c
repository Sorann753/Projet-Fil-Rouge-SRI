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