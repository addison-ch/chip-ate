#ifndef CHIP8KEYBOARD_H
#define CHIP8KEYBOARD_H

#define TOTAL_KEYS 16

#include <stdbool.h>

struct keyboard {
    bool keys[TOTAL_KEYS];
};

int convert_key(const char* map, char key);
void keyboard_press(struct keyboard* kb, int key);
void keyboard_lift(struct keyboard* kb, int key);
bool keyboard_is_pressed(struct keyboard* kb, int key);

#endif
