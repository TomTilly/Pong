//
//  main.c
//  Pong
//
//  Created by Thomas Tillistrand on 5/2/19.
//  Copyright © 2019 Thomas Tillistrand. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

int main() {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "Pong");
    
    while (1) {
        while (SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT){
                exit(0);
            }
        }
    }
    return 0;
}
