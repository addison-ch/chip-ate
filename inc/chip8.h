#ifndef CHIP8_H
#define CHIP8_H

#include "config.h"
#include "chip8_memory.h"
#include "registers.h"
#include "stack.h"

struct chip8 {
 struct chip8_memory memory;
 struct registers registers;
 struct stack stack;

};

#endif