#ifndef CHIP8MEMORY_H
#define CHIP8MEMORY_H

#include "config.h"


struct memory {
    unsigned char RAM[MEMORY_SIZE];
};

void memory_set (struct memory* mem, int index, unsigned char val);
unsigned char memory_get (struct memory* mem, int index);
unsigned short memory_get_opcode(struct memory* mem, int index);

#endif