#ifndef CHIP8SCREEN_H
#define CHIP8SCREEN_H

#include <stdbool.h>
#include "config.h"

struct screen {
    bool display[HEIGHT][WIDTH];
};

void toggle_pixel(struct screen* screen, int x, int y);
bool check_pixel(struct screen* screen, int x, int y);

#endif