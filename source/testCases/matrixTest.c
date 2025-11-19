#include "testCases/matrixTest.h"

void testMatrix(void){
    Matrix double_M = initMatrix(6, 6, sizeof(double));

    double_matrixSet(&double_M, 5, 5, 12.5);

    double_matrixPrint(&double_M);

    freeMatrix(&double_M);
}