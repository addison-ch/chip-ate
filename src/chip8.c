#include "chip8.h"
#include <memory.h>
#include <assert.h>
#include "config.h"

const char font_set[80] = {
    0xf0, 0x90, 0x90, 0x90, 0xf0,
    0x20, 0x60, 0x20, 0x20, 0x70,
    0xf0, 0x10, 0xf0, 0x80, 0xf0,
    0xf0, 0x10, 0xf0, 0x10, 0xf0,
    0x90, 0x90, 0xf0, 0x10, 0x10,
    0xf0, 0x80, 0xf0, 0x10, 0xf0,
    0xf0, 0x80, 0xf0, 0x90, 0xf0,
    0xf0, 0x10, 0x20, 0x40, 0x40,
    0xf0, 0x90, 0xf0, 0x90, 0xf0,
    0xf0, 0x90, 0xf0, 0x10, 0xf0,
    0xf0, 0x90, 0xf0, 0x90, 0x90,
    0xe0, 0x90, 0xe0, 0x90, 0xe0,
    0xf0, 0x80, 0x80, 0x80, 0xf0,
    0xe0, 0x90, 0x90, 0x90, 0xe0,
    0xf0, 0x80, 0xf0, 0x80, 0xf0, 
    0xf0, 0x80, 0xf0, 0x80, 0x80
};

void chip8_initialize(struct chip8* chip8) {

    memset(chip8, 0, sizeof(struct chip8));
    memcpy(chip8->memory.RAM, font_set, sizeof(font_set));
}

void chip8_load(struct chip8* chip8, const char* buf, size_t size) {
    assert(size+LOAD_ADDRESS < MEMORY_SIZE);

    memcpy(&chip8->memory.RAM[LOAD_ADDRESS], buf, size);
    chip8->registers.PC = LOAD_ADDRESS;
}

void chip8_execute(struct chip8* chip8, unsigned short opcode) {
    switch(opcode) {

        // 00E0 - CLS : Clears the screen
        case 0x00E0: 
            clear_screen(&chip8->screen);
        break;

        // 00EE - Ret : Return from a subroutine
        case 0x00EE:
            chip8->registers.PC = stack_pop(chip8);
        break;

        default:
            chip8_exec_extended(chip8, opcode);

    }
}

static void chip8_exec_extended(struct chip8* chip8, unsigned short opcode) {
    
    unsigned short nnn = opcode & 0x0fff;
    unsigned char x = (opcode >> 8) & 0x000f;
    unsigned char y = (opcode >> 4) & 0x000f;
    unsigned char kk = opcode & 0x00ff;
    unsigned char n = opcode & 0x000f;

    switch(opcode & 0xf000) {

        // 1NNN - JP addr : Jumps to address NNN
        case 0x1000:
            chip8->registers.PC = nnn;
        break;

        // 2NNN - CALL addr : Call subroutine at location nnn
        case 0x2000:
            stack_push(chip8, chip8->registers.PC);
            chip8->registers.PC = nnn;
        break;

        // 3xkk - SE Vx, byte : Skip next instruction if Vx=kk
        case 0x3000:
            if (chip8->registers.V[x] == kk) {
                chip8->registers.PC += 2;
            }
        break;

         // 4xkk - SNE Vx, byte : Skip next instruction if Vx!=kk
        case 0x4000:
            if (chip8->registers.V[x] != kk) {
                chip8->registers.PC += 2;
            }
        break;

        // 5xy0 - SE, Vx, Vy : Skip the next instruction if Vx = Vy
        case 0x5000:
            if (chip8->registers.V[x] == chip8->registers.V[y]) {
                chip8->registers.PC += 2;
            }
        break;

        // 6xkk - LD Vx, byte : Sets/loads Vx = kk
        case 0x6000:
            chip8->registers.V[x] = kk;
        break;

        // 7xkk - ADD Vx, byte : Sets Vx = Vx + kk
        case 0x7000:
            chip8->registers.V[x] += kk;
        break;

        // 8xy0 - LD Vx, Vy : Loads the value of register Vy into Vx
        case 0x8000:
            chip8_exec_extended_eight(chip8, opcode);
        break;

        // 9xy0 - SNE Vx, Vy. Skip next instruction if Vx != Vy
        case 0x9000:
            if (chip8->registers.V[x] != chip8->registers.V[y])
            {
                chip8->registers.PC += 2;
            }
        break;


    }
}