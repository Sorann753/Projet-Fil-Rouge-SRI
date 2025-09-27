#ifndef MARIO_H
#define MARIO_H

#include <stdint.h>
#include <stdlib.h>

#include "items.h"



typedef enum {
    Mario_Mario,
    Mario_SuperMario,
    Mario_GigaMario,
    Mario_FireMario,
    Mario_CapeMario,

    Mario_DeadMario,
    __Mario_CountOfStates,
} MarioStates;

struct Mario{
    MarioStates currentState;
    uint16_t coins;
    void (*on_collision)(struct Mario* const self, enum ItemKind obj);
};
typedef struct Mario Mario;



void marioCollision(Mario* const self, enum ItemKind obj);

void superMarioCollision(Mario* const self, enum ItemKind obj);

void gigaMarioCollision(Mario* const self, enum ItemKind obj);

#endif