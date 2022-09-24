#include "keyboard.h"

int convert_key (const char* map, char key) {
    for (int i = 0; i < TOTAL_KEYS; i++) {
        if (map[i] == key) {
            return i;
        }
    }
    return -1;
}