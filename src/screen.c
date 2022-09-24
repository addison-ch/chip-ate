#include "screen.h"
#include <assert.h>


static void check_bounds(int x, int y) {
    assert(x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT);
}

void toggle_pixel(struct screen* screen, int x, int y) {
    check_bounds(x, y);
    screen->display[y][x] = true;
}

bool check_pixel(struct screen* screen, int x, int y) {
    check_bounds(x, y);
    return screen->display[y][x];
}