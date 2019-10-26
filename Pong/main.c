//
//  main.c
//  Pong
//
//  Created by Thomas Tillistrand on 5/2/19.
//  Copyright © 2019 Thomas Tillistrand. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

#define WINDOW_W 640
#define WINDOW_H 480

#define PADDLE_W    16
#define PADDLE_H    96
#define PADDLE1_X   PADDLE_W // paddle margin = paddle width
#define PADDLE2_X   WINDOW_W - (PADDLE_W * 2)
#define PADDLE_SPEED 3
#define BALL_D      20
#define BALL_SPEED 3

typedef unsigned char byte;

typedef struct {
    SDL_Rect rect;
    int score;
} paddle_t;

typedef struct {
    SDL_Rect rect;
    int dx;
    int dy;
} ball_t;

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
    ball_t ball;
    
    
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Pong", 0, 0, WINDOW_W, WINDOW_H, 0);
    renderer = SDL_CreateRenderer(window, -1, flags);
    
    keystate = SDL_GetKeyboardState(NULL);
    
    srand((unsigned)time(NULL));
    
    for(int i = 0; i < NUMPADDLES; i++) {
        // Setting initial paddle position
        paddles[i].rect.x = (i == left) ? PADDLE1_X : PADDLE2_X;
        paddles[i].rect.y = (WINDOW_H / 2) - (PADDLE_H / 2);
        paddles[i].rect.w = PADDLE_W;
        paddles[i].rect.h = PADDLE_H;
    }
    
    ball.rect.x = WINDOW_W / 2;
    ball.rect.y = WINDOW_H / 2;
    ball.rect.w = BALL_D;
    ball.rect.h = BALL_D;
    ball.dx = 0;
    ball.dy = 0;
    
    while (1) {
        // Check for input
        while (SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT){
                exit(0);
            }
            
            if ( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
                if(rand() % 2){
                    ball.dx = BALL_SPEED;
                    ball.dy = -BALL_SPEED;
                } else {
                    ball.dx = -BALL_SPEED;
                    ball.dy = BALL_SPEED;
                }
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
        
        
        // Update game
        
        ball.rect.x += ball.dx;
        ball.rect.y += ball.dy;
        
        if(ball.rect.x < 0){
            ball.dx = -ball.dx;
            ball.rect.x = 0;
        }
        if(ball.rect.x > WINDOW_W - ball.rect.w){
            ball.dx = -ball.dx;
            ball.rect.x = WINDOW_W - ball.rect.w;
        }
        if(ball.rect.y < 0){
            ball.dy = -ball.dy;
            ball.rect.y = 0;
        }
        if(ball.rect.y > WINDOW_H - ball.rect.h){
            ball.dy = -ball.dy;
            ball.rect.y = WINDOW_H - ball.rect.h;
        }
        
        // Draw everything
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        SDL_SetRenderDrawColor(renderer, 3, 80, 233, 255);
        SDL_RenderFillRect(renderer, &paddles[left].rect);
        SDL_RenderFillRect(renderer, &paddles[right].rect);
        
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &ball.rect);
        
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }
    return 0;
}
