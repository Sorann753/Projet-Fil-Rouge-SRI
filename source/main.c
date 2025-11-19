#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "utils/matrix_impl.h"

#define SIMULATOR
// #define HARDWARE

int main(void){
    #if defined (SIMULATOR) && defined (HARDWARE)
    // you are not supposed to enable both at once
    assert(false); 
    #endif

    printf("Hello world !\n");
    return 0;
}
