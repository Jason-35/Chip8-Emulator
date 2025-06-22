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

    return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppIterate(void *appstate) {
    
    SDL_Delay(100);
    emulateCycle(&cpu);
   
    SDL_FRect r;
    r.w = ASPECT;
    r.h = ASPECT;
    // draw bg
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // draw pixel
    
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
    
    return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    if (event->type == SDL_EVENT_KEY_DOWN ||
        event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

