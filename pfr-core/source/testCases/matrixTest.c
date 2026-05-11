#include "testCases/matrixTest.h"

void testDoubleMatrix(void){
    printf("----- TEST DOUBLE MATRIX -----\n");
    Matrix double_M = initMatrix(6, 6, sizeof(double));

    double_matrixSet(&double_M, 5, 5, 12.5);
    double changedVal = double_matrixGet(&double_M, 5, 5);
    assert(changedVal < 12.500001 && changedVal > 12.499999);

    double_matrixPrint(&double_M);

    freeMatrix(&double_M);
    printf("----- TEST SUCCESS -----\n\n");
}

void testByteMatrix(void){
    printf("----- TEST BYTE MATRIX -----\n");
    Matrix byte_M = initMatrix(7, 7, sizeof(byte));

    printf("%ld\n", sizeof(Matrix));

    byte_matrixSet(&byte_M, 3, 3, 0xFF);
    assert(byte_matrixGet(&byte_M, 3, 3) == 0xFF);

    byte_matrixPrint(&byte_M);

    freeMatrix(&byte_M);
    printf("----- TEST SUCCESS -----\n\n");
}