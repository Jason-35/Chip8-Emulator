#define SDL_MAIN_USE_CALLBACKS 1
#include "chip8.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WINDOW_HEIGHT 32
#define WINDOW_WIDTH 64
#define ASPECT 16
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
Chip8 cpu;
Uint32 last_cycle_time;
Uint32 last_render_cycle_time;
const int instruction_per_second = 750;
const double cycle_per_second = 1000/instruction_per_second;
const int frame_per_second = 60;
const double render_per_second = 1000 / frame_per_second;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    
    if (argc == 1) {
        printf("rom file missing\n");
        return SDL_APP_FAILURE;
    }
   
    cpu = initalize_chip8();
    load_rom(&cpu, argv[1]);
    if (!SDL_CreateWindowAndRenderer("Chip 8", 1024, 512, 0, &window, &renderer)) {
        printf("Couldnt create window\n");
        return SDL_APP_FAILURE;   
    }

    last_cycle_time = SDL_GetTicks();
    last_render_cycle_time = SDL_GetTicks();
    return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppIterate(void *appstate) {
    
    Uint32 current_cycle_time = SDL_GetTicks();

    if ((current_cycle_time - last_cycle_time) >= cycle_per_second) {
        emulateCycle(&cpu);
        last_cycle_time = current_cycle_time;
    }
   
    SDL_FRect r;
    r.w = ASPECT;
    r.h = ASPECT;
    // draw bg
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        // draw pixel
    if ((current_cycle_time - last_render_cycle_time) >= render_per_second) {
        SDL_RenderClear(renderer);
        for (int i = 0; i < GRAPHIC_SIZE; i++) {
            if (cpu.graphic[i] == 1) {
                r.x = (i % 64) * ASPECT;
                r.y = (i / 64) * ASPECT;
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &r);
            }
        }
       // -----
        // render everything
        SDL_RenderPresent(renderer);
        last_render_cycle_time = current_cycle_time;
    }
    SDL_Delay(1);
    
    return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {

       
    if ((event -> type == SDL_EVENT_KEY_DOWN)) {
        switch(event -> key.scancode) {
            case SDL_SCANCODE_P:
                return SDL_APP_SUCCESS;
            case SDL_SCANCODE_1:
                cpu.key[0] = 1;
                break;
            case SDL_SCANCODE_2:
                cpu.key[1] = 1;
                break;
            case SDL_SCANCODE_3:
                cpu.key[2] = 1;
                break;
            case SDL_SCANCODE_4:
                cpu.key[3] = 1;
                break;
            case SDL_SCANCODE_Q:
                cpu.key[4] = 1;
                break;
            case SDL_SCANCODE_W:
                cpu.key[5] = 1;
                break;
            case SDL_SCANCODE_E:
                cpu.key[6] = 1;
                break;
            case SDL_SCANCODE_R:
                cpu.key[7] = 1;
                break;
            case SDL_SCANCODE_A:
                cpu.key[8] = 1;
                break;
            case SDL_SCANCODE_S:
                cpu.key[9] = 1;
                break;
            case SDL_SCANCODE_D:
                cpu.key[10] = 1;
                break;
            case SDL_SCANCODE_F:
                cpu.key[11] = 1;
                break;
            case SDL_SCANCODE_Z:
                cpu.key[12] = 1;
                break;
            case SDL_SCANCODE_X:
                cpu.key[13] = 1;
                break;
            case SDL_SCANCODE_C:
                cpu.key[14] = 1;
                break;
            case SDL_SCANCODE_V:
                cpu.key[15] = 1;
                break;
        };
    }

     if ((event -> type == SDL_EVENT_KEY_UP)) {
        switch(event -> key.scancode) {
            case SDL_SCANCODE_1:
                cpu.key[0] = 0;
                break;
            case SDL_SCANCODE_2:
                cpu.key[1] = 0;
                break;
            case SDL_SCANCODE_3:
                cpu.key[2] = 0;
                break;
            case SDL_SCANCODE_4:
                cpu.key[3] = 0;
                break;
            case SDL_SCANCODE_Q:
                cpu.key[4] = 0;
                break;
            case SDL_SCANCODE_W:
                cpu.key[5] = 0;
                break;
            case SDL_SCANCODE_E:
                cpu.key[6] = 0;
                break;
            case SDL_SCANCODE_R:
                cpu.key[7] = 0;
                break;
            case SDL_SCANCODE_A:
                cpu.key[8] = 0;
                break;
            case SDL_SCANCODE_S:
                cpu.key[9] = 0;
                break;
            case SDL_SCANCODE_D:
                cpu.key[10] = 0;
                break;
            case SDL_SCANCODE_F:
                cpu.key[11] = 0;
                break;
            case SDL_SCANCODE_Z:
                cpu.key[12] = 0;
                break;
            case SDL_SCANCODE_X:
                cpu.key[13] = 0;
                break;
            case SDL_SCANCODE_C:
                cpu.key[14] = 0;
                break;
            case SDL_SCANCODE_V:
                cpu.key[15] = 0;
                printf("reg");
                break;
        };
    }


    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

