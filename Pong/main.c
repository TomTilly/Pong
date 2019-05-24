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

#define WINDOW_W 640
#define WINDOW_H 480

#define PADDLE_W 16
#define PADDLE_H 96
#define PADDLE1_X PADDLE_W // paddle margin = paddle width
#define PADDLE_SPEED 3

typedef unsigned char byte;

int main() {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    const byte* keystate;
    SDL_RendererFlags flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Pong", 0, 0, WINDOW_W, WINDOW_H, 0);
    renderer = SDL_CreateRenderer(window, -1, flags);
    
    keystate = SDL_GetKeyboardState(NULL);
    
    // Setting initial paddle position
    SDL_Rect paddle1;
    paddle1.x = PADDLE1_X;
    paddle1.y = (WINDOW_H / 2) - (PADDLE_H / 2);
    paddle1.w = PADDLE_W;
    paddle1.h = PADDLE_H;
    
    while (1) {
        // Check for input
        while (SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT){
                exit(0);
            }
        }
        if(keystate[SDL_SCANCODE_W]) {
            paddle1.y -= PADDLE_SPEED;
        }
        if(keystate[SDL_SCANCODE_S]) {
            paddle1.y += PADDLE_SPEED;
        }
        
        // Process game
        
        // Draw everything
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        SDL_SetRenderDrawColor(renderer, 3, 80, 233, 255);
        SDL_RenderFillRect(renderer, &paddle1);
        
        SDL_RenderPresent(renderer);
        
        SDL_Delay(10);
    }
    return 0;
}
