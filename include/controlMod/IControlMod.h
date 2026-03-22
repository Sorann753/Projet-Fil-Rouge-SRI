#ifndef ICONTROL_MOD_H
#define ICONTROL_MOD_H

typedef struct IControlMod_t {
    void (*getAction)(void*);
    void (*updateState)(void*);
} IControlMod;

#endif