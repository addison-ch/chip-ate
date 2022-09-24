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

bool draw_sprite(struct screen* screen, int x, int y, const char* sprite, int num) {
    bool pixel_overlap = false;

    for (int line_y = 0; line_y < num; line_y++) {

        char px = sprite[ line_y];

        for (int  line_x = 0;  line_x < 8;  line_x++) {

            // checks if the current pixel is set to 1
            // shift the 1 to the current spot we are looking at
            // if its false/0 it means that the pixel is set to 0
            // and nothing should be printed
            if ((px & (0b10000000 >>  line_x)) == 0) {
                continue; // skip to next pixel
            }
            
            // modulo handles overflow from the edges
            int y_offset = (line_y+y) % HEIGHT;
            int x_offset = (line_x+x) % WIDTH;

            if (screen->display[y_offset][x_offset]) {
                pixel_overlap = true;
            }

            // update pixel value using XOR
            screen->display[y_offset][x_offset] ^= true;
        }
    }
    
    return pixel_overlap;
}