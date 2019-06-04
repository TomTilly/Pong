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

#define PADDLE_W    16
#define PADDLE_H    96
#define PADDLE1_X   PADDLE_W // paddle margin = paddle width
#define PADDLE2_X   WINDOW_W - (PADDLE_W * 2)
#define PADDLE_SPEED 3

typedef unsigned char byte;

typedef struct {
    SDL_Rect rect;
    int score;
} paddle_t;

enum { left, right, NUMPADDLES };
typedef enum { false, true } bool;

void MovePaddle(paddle_t *paddle, int speed) {
    paddle->rect.y += speed;
    if ( paddle->rect.y < 0 ) {
        paddle->rect.y = 0;
    } else if ( paddle->rect.y > (WINDOW_H - PADDLE_H) ) {
        paddle->rect.y = WINDOW_H - PADDLE_H;
    }
}

int main() {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    const byte* keystate;
    SDL_RendererFlags flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    paddle_t paddles[NUMPADDLES];
    
    
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Pong", 0, 0, WINDOW_W, WINDOW_H, 0);
    renderer = SDL_CreateRenderer(window, -1, flags);
    
    keystate = SDL_GetKeyboardState(NULL);
    
    for(int i = 0; i < NUMPADDLES; i++) {
        // Setting initial paddle position
        paddles[i].rect.x = (i == left) ? PADDLE1_X : PADDLE2_X;
        paddles[i].rect.y = (WINDOW_H / 2) - (PADDLE_H / 2);
        paddles[i].rect.w = PADDLE_W;
        paddles[i].rect.h = PADDLE_H;
    }
    
    
    while (1) {
        // Check for input
        while (SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT){
                exit(0);
            }
        }
        
        if ( keystate[SDL_SCANCODE_W] ) {
            MovePaddle(&paddles[left], -PADDLE_SPEED);
        }
        if ( keystate[SDL_SCANCODE_S] ) {
            MovePaddle(&paddles[left], PADDLE_SPEED);
        }
        
        if ( keystate[SDL_SCANCODE_UP] ) {
            MovePaddle(&paddles[right], -PADDLE_SPEED);
        }
        
        if ( keystate[SDL_SCANCODE_DOWN] ) {
            MovePaddle(&paddles[right], PADDLE_SPEED);
        }
        
        // Process game
        
        // Draw everything
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        SDL_SetRenderDrawColor(renderer, 3, 80, 233, 255);
        SDL_RenderFillRect(renderer, &paddles[left].rect);
        SDL_RenderFillRect(renderer, &paddles[right].rect);
        SDL_RenderPresent(renderer);
        
        SDL_Delay(10);
    }
    return 0;
}
