#include <assert.h>
#include "stack.h"
#include "chip8.h"


static void stack_in_bounds(struct chip8* chip8) {
    assert(chip8->registers.SP < STACK_DEPTH);
}

void stack_push(struct chip8* chip8, unsigned short val) {
    stack_in_bounds(chip8);
    chip8->stack.stack[chip8->registers.SP] = val;
    chip8->registers.SP += 1;
}

unsigned short stack_pop(struct chip8* chip8) {
    chip8->registers.SP -= 1;
    stack_in_bounds(chip8);
    unsigned short val = chip8->stack.stack[chip8->registers.SP];
    return val;
}