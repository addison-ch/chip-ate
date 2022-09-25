#include <assert.h>
#include "memory.h"
#include "config.h"

void memory_set(struct memory* mem, int index, unsigned char val) {
    assert(index >= 0 && index < MEMORY_SIZE);
    mem->RAM[index] = val;
}

unsigned char memory_get(struct memory* mem, int index) {
    assert(index >= 0 && index < MEMORY_SIZE);
    return mem->RAM[index];
}

unsigned short memory_get_opcode(struct memory* mem, int index) {
    unsigned char byte1 = memory_get(mem, index);
    unsigned char byte2 = memory_get(mem, index+1);
    
    // extends to the length of 2 bytes
    return byte1 << 8 | byte2;
}