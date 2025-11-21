#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "utils/matrix_impl.h"

#define SIMULATOR
// #define HARDWARE

int main(void){
    #if defined (SIMULATOR) && defined (HARDWARE)
    #error you are not supposed to enable both at once
    #endif

    printf("Hello world !\n");
    return 0;
}
