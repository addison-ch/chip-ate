#include "keyboard.h"

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

void keyboard_toggle (struct keyboard* kb, int key) {
    if (kb->keys[key] == false) {
        kb->keys[key] = true;
    } else {
        kb->keys[key] = false;
    }
}

// void keyboard_lift (struct keyboard* kb, int key) {
//     kb->keys[key] = false;
// }

bool keyboard_is_pressed (struct keyboard* kb, int key) {
    return kb->keys[key];
}