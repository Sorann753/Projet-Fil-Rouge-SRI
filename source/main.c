#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "matrix_impl.h"

#define SIMULATOR
// #define HARDWARE

int main(void){
    #if defined (SIMULATOR) && defined (HARDWARE)
    // you are not supposed to enable both at once
    assert(false); 
    #endif

    Matrix double_M = initMatrix(15, 30, sizeof(double));

    double_matrixSet(&double_M, 5, 5, 12.5);

    printf("valeur : %f\n", double_matrixGet(&double_M, 5, 5));

    freeMatrix(&double_M);

    printf("Hello world !\n");
    return 0;
}
