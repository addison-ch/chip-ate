#include <memory.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "config.h"
#include "chip8.h"
#include "SDL2/SDL.h"


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

static char wait_for_key_press(struct chip8* chip8)
{
    SDL_Event event;
    while(SDL_WaitEvent(&event))
    {
        if (event.type != SDL_KEYDOWN) {
            continue;
        }
        
        char c = event.key.keysym.sym;
        char chip8_key = convert_key(&chip8->kb, c);
        if (chip8_key != -1)
        {
            return chip8_key;
        }
    }

    return -1;
}

// Below -> Actual interpreting of the opcodes
// See : http://devernay.free.fr/hacks/chip8/C8TECH10.HTM

static void exec_F(struct chip8* chip8, unsigned short opcode) {
unsigned char x = (opcode >> 8) & 0x000f;
    switch (opcode & 0x00ff) {
        case 0x07:
            chip8->registers.V[x] = chip8->registers.delay_timer;
            break;
        case 0x0A:
            char pressed_key = wait_for_key_press(chip8);
            chip8->registers.V[x] = pressed_key;
            break; 
        case 0x15:
            chip8->registers.delay_timer = chip8->registers.V[x];
            break;
        case 0x18:
            chip8->registers.sound_timer = chip8->registers.V[x];
            break;
        case 0x1e:
            chip8->registers.I += chip8->registers.V[x];
            break;
        case 0x29:
            chip8->registers.I = chip8->registers.V[x] * CHIP8_DEFAULT_SPRITE_HEIGHT;
            break;
        case 0x33:
            unsigned char hundreds = chip8->registers.V[x] / 100;
            unsigned char tens = chip8->registers.V[x] / 10 % 10;
            unsigned char units = chip8->registers.V[x] % 10;
            memory_set(&chip8->memory, chip8->registers.I, hundreds);
            memory_set(&chip8->memory, chip8->registers.I+1, tens);
            memory_set(&chip8->memory, chip8->registers.I+2, units);
            break;
        case 0x55:
            for (int i = 0; i <= x; i++) {
                memory_set(&chip8->memory, chip8->registers.I+i, chip8->registers.V[i]);
            }
            break;
        case 0x65:
            for (int i = 0; i <= x; i++) {
                chip8->registers.V[i] = memory_get(&chip8->memory, chip8->registers.I+i);
            }
            break;

    }
}

static void exec_eight(struct chip8* chip8, unsigned short opcode) {
    unsigned char x = (opcode >> 8) & 0x000f;
    unsigned char y = (opcode >> 4) & 0x000f;
    unsigned char last_hex = opcode & 0x000f;
    unsigned short tmp = 0;
    switch(last_hex) {
        case 0x00:
            chip8->registers.V[x] = chip8->registers.V[y];
            break;
        case 0x01:
            chip8->registers.V[x] = chip8->registers.V[x] | chip8->registers.V[y];
            break;
        case 0x02:
            chip8->registers.V[x] = chip8->registers.V[x] & chip8->registers.V[y];
            break;
        case 0x03:
            chip8->registers.V[x] = chip8->registers.V[x] ^ chip8->registers.V[y];
            break;
        case 0x04:
            tmp = chip8->registers.V[x] + chip8->registers.V[y];
            chip8->registers.V[0x0f] = false;
            if (tmp > 0xff) {
                chip8->registers.V[0x0f] = true;
            }
            chip8->registers.V[x] = tmp;
            break;
        case 0x05:
            chip8->registers.V[0x0f] = false;
            if (chip8->registers.V[x] > chip8->registers.V[y])
            {
                chip8->registers.V[0x0f] = true;
            }
            chip8->registers.V[x] = chip8->registers.V[x] - chip8->registers.V[y];
            break;
        case 0x06:
            chip8->registers.V[0x0f] = chip8->registers.V[x] & 0b00000001;
            chip8->registers.V[x] /= 2;
            break;
        case 0x07:
            chip8->registers.V[0x0f] = chip8->registers.V[y] > chip8->registers.V[x];
            chip8->registers.V[x] = chip8->registers.V[y] - chip8->registers.V[x];
        break;
        case 0x0E:
            chip8->registers.V[0x0f] = chip8->registers.V[x] & 0b10000000;
            chip8->registers.V[x] = chip8->registers.V[x] * 2;
        break;
    }
}

static void exec_extend(struct chip8* chip8, unsigned short opcode) {
    unsigned short nnn = opcode & 0x0fff;
    unsigned char x = (opcode >> 8) & 0x000f;
    unsigned char y = (opcode >> 4) & 0x000f;
    unsigned char kk = opcode & 0x00ff;
    unsigned char n = opcode & 0x000f;

    switch(opcode & 0xf000) {
        case 0x1000:
            chip8->registers.PC = nnn;
            break;
        case 0x2000:
            stack_push(chip8, chip8->registers.PC);
            chip8->registers.PC = nnn;
            break;
        case 0x3000:
            if (chip8->registers.V[x] == kk) {
                chip8->registers.PC += 2;
            }
            break;
        case 0x4000:
            if (chip8->registers.V[x] != kk) {
                chip8->registers.PC += 2;
            }
            break;
        case 0x5000:
            chip8->registers.PC += chip8->registers.V[x] == chip8->registers.V[y] ? 2 : 0;
            break;
        case 0x6000:
            chip8->registers.V[x] = kk;
            break;
        case 0x7000:
            chip8->registers.V[x] += kk;
            break;
        case 0x8000:
            exec_eight(chip8, opcode);
            break;
        case 0x9000:
            chip8->registers.PC += (chip8->registers.V[x] != chip8->registers.V[y]) ? 2 : 0;
            break;
        case 0xA000:
            chip8->registers.I = nnn;
            break;
        case 0xB000:
            chip8->registers.PC = nnn + chip8->registers.V[0];
            break;
        case 0xC000:
            chip8->registers.V[x] = (rand() % 0xff) & kk;
            break;
        case 0xD000:
            const char* sprite = (const char*) &chip8->memory.RAM[chip8->registers.I];
            chip8->registers.V[0x0f] = draw_sprite(&chip8->screen, chip8->registers.V[x], chip8->registers.V[y], sprite, n);
            break;
        case 0xE000:
            unsigned short part = opcode & 0x00ff;
            if (keyboard_is_pressed(&chip8->kb, chip8->registers.V[x])) {
                if (part == 0x9E) {
                    chip8->registers.PC += 2;
                } 
            } else {
                if (part == 0xA1) {
                    chip8->registers.PC += 2;
                }
            }
            break;
        case 0xF000:
            exec_F(chip8, opcode);
            break;
        default:
            break;

    }
}

void execute(struct chip8* chip8, unsigned short opcode) {
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
            exec_extend(chip8, opcode);
    }
}

