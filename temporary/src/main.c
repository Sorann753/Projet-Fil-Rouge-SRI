#include <stdio.h>

#include "items.h"
#include "mario.h"

int main(int argc, char** argv) {

    Mario mario = {
        .currentState = Mario_GigaMario,
        .on_collision = &gigaMarioCollision,
        .coins = 0,
    };

    int obj = Object_Bomb;

    int i = 0;
    while (mario.currentState != Mario_DeadMario && i < 10) {
        printf("[COLLISION !]\n");
        mario.on_collision(&mario, obj);

        i++;
    }

    printf("GAME OVER!\n");
    
    return 0;
}
