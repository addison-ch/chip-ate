#include <assert.h>
#include "chip8_memory.h"

static void memory_check_bounds(int index) {
    assert(index >= 0 && index < CHIP8_MEMORY_SIZE);
}

void chip8_memory_set(struct chip8_memory* memory, int index, unsigned char val) {
    memory_check_bounds(index);
    memory->memory[index] = val;
}

unsigned char chip8_memory_get(struct chip8_memory* memory, int index) {
    memory_check_bounds(index);
    return memory->memory[index];
}
