#ifndef CHIP8KEYBOARD_H
#define CHIP8KEYBOARD_H

#define TOTAL_KEYS 16

#include <stdbool.h>

struct keyboard {
    bool keys[TOTAL_KEYS];
    const char* mapping;
};

void set_map(struct keyboard* kb, const char* map);
int convert_key(struct keyboard* kb, char key);
void keyboard_press(struct keyboard* kb, int key);
void keyboard_lift(struct keyboard* kb, int key);
bool keyboard_is_pressed(struct keyboard* kb, int key);

#endif
