#ifndef CHIP8_H
#define CHIP8_H

#include "config.h"
#include "memory.h"
#include "registers.h"
#include "stack.h"
#include "keyboard.h"
#include "screen.h"
#include <stddef.h>

struct chip8 {
    struct memory memory;
    struct registers registers;
    struct stack stack;
    struct keyboard kb;
    struct screen screen;
};

void chip8_initialize(struct chip8* chip8);
void chip8_load(struct chip8* chip8, const char* buf, size_t size); // load buffer into memory
void chip8_execute(struct chip8* chip8, unsigned short opcode);

#endif