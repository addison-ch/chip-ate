#ifndef CHIP8REGISTERS_H
#define CHIP8REGISTERS_H

#include "config.h"

struct registers {
    unsigned char V[NUM_DATA_REGISTERS];
    unsigned short I;
    unsigned char delay_timer;
    unsigned char sound_timer;
    unsigned short PC;
    unsigned short SP;
};

#endif