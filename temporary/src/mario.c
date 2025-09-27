#include "mario.h"

void marioCollision(Mario* const self, enum ItemKind obj){
    switch(obj){
        case Object_Shell:
            self->currentState = Mario_DeadMario;
            self->on_collision = NULL;
            break;
        
        case Object_Bomb:
            self->currentState = Mario_DeadMario;
            self->on_collision = NULL;
            break;

        case Object_Mushroom:
            self->currentState = Mario_SuperMario;
            self->on_collision = &superMarioCollision;
            break;

        default:
            self->coins++;
    }
} 

void superMarioCollision(Mario* const self, enum ItemKind obj){
    switch(obj){
        case Object_Shell:
            self->currentState = Mario_Mario;
            self->on_collision = &marioCollision;
            break;

        case Object_Bomb:
            self->currentState = Mario_Mario;
            self->on_collision = &marioCollision;
            self->coins = 0;
            break;

        case Object_Mushroom:
            self->currentState = Mario_GigaMario;
            self->on_collision = &gigaMarioCollision;
            break;

        default:
            self->coins++;
    }
}

void gigaMarioCollision(Mario* const self, enum ItemKind obj){
    switch(obj){
        case Object_Bomb:
            self->currentState = Mario_SuperMario;
            self->on_collision = &superMarioCollision;
            break;

        default:
            self->coins++;
    }
}