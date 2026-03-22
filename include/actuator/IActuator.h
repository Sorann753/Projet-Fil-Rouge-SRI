#ifndef IACTUATOR_H
#define IACTUATOR_H

typedef struct IActuator_t{
    void (*init)(void*);
    void (*read_config)(void*);
    void (*forward)(void*);
    void (*backward)(void*);
    void (*turn)(void*);
} IActuator;

#endif