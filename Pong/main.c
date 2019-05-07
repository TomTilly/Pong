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

int main() {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_W, WINDOW_H, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "Pong");
    
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
