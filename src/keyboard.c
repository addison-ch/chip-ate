#include "keyboard.h"
#include <assert.h>

static void check_key_bounds (int key) {
    assert(key >= 0 && key < TOTAL_KEYS);
}

int convert_key (const char* map, char key) {
    for (int i = 0; i < TOTAL_KEYS; i++) {
        if (map[i] == key) {
            return i;
        }
    }
    return -1;
}

void keyboard_press (struct keyboard* kb, int key) {
    kb->keys[key] = true;

}

void keyboard_lift (struct keyboard* kb, int key) {
    kb->keys[key] = false;
}

bool keyboard_is_pressed (struct keyboard* kb, int key) {
    return kb->keys[key];
}