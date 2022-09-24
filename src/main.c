#include <stdio.h>
#include <stdbool.h>
#include "SDL2/SDL.h"
#include "chip8.h"
#include "keyboard.h"

const char keyboard_map[TOTAL_KEYS] = {
    SDLK_0, SDLK_1, SDLK_2, 
    SDLK_3, SDLK_4, SDLK_5,
    SDLK_6, SDLK_7, SDLK_8, 
    SDLK_9, SDLK_a, SDLK_b,
    SDLK_c, SDLK_d, SDLK_e, 
    SDLK_f
};

int main (int argc, char **argv) {

    struct chip8 chip8;
    chip8_initialize(&chip8);
    
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow(
        EMULATOR_WINDOW_TITLE,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, 
        CHIP8_WIDTH * EMULATOR_SCALE, 
        CHIP8_HEIGHT * EMULATOR_SCALE, 
        SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);
    while (1) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                goto out;
            } else if (event.type == SDL_KEYDOWN) {
                char k = event.key.keysym.sym;
                int mapped_key = convert_key(keyboard_map, k);
                if (mapped_key != -1) {
                    keyboard_press(&chip8.kb, mapped_key);
                }
            } else if (event.type == SDL_KEYUP) {
                char k = event.key.keysym.sym;
                int mapped_key = convert_key(keyboard_map, k);
                if (mapped_key != -1) {
                    keyboard_lift(&chip8.kb, mapped_key);
                }

            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 20, 80, 30, 0);

        SDL_Rect r;
        r.x = 0;
        r.y = 0;
        r.w = 40;
        r.h = 40;

        SDL_RenderFillRect(renderer, &r);
        SDL_RenderPresent(renderer);
    }
    out:
    SDL_DestroyWindow(window);
    return 0;
}