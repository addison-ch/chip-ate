#include "keyboard.h"
#include <assert.h>

static void check_key_bounds (int key) {
    assert(key >= 0 && key < TOTAL_KEYS);
}

void kb_set_map (struct keyboard* kb, const char* map) {
    kb->mapping = map;
}

int convert_key (struct keyboard* kb, char key) {
    for (int i = 0; i < TOTAL_KEYS; i++) {
        if (kb->mapping[i] == key) {
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