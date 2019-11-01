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
#include <SDL2_ttf/SDL_ttf.h>
#include "sound.h"

#define WINDOW_W        640
#define WINDOW_H        480
#define SCALE           2

#define PADDLE_W        16
#define PADDLE_H        96
#define PADDLE1_X       PADDLE_W // paddle margin = paddle width
#define PADDLE2_X       WINDOW_W - (PADDLE_W * 2)
#define PADDLE_SPEED    3
#define BALL_D          20
#define BALL_SPEED      3
#define MARGIN          40

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




// call whenever a player's score is updated in order to update the score texture
void UpdateScore(int side, SDL_Texture **score_texture, TTF_Font *font, SDL_Renderer *renderer, int *score_to_update, int new_score, SDL_Rect *rect){
    SDL_Surface *score_surface;
    SDL_Color color = { 255,255,255,255 };
    
    // create a string from player's score
    *score_to_update = new_score;
    char text[16];
    sprintf(&text[0], "%d", *score_to_update);
    
    // update the texture with new score value
    score_surface = TTF_RenderText_Solid(font, &text[0], color);
    rect->w = score_surface->w;
    rect->h = score_surface->h;
    if(side == right){
        rect->x = WINDOW_W - MARGIN - rect->w;
    }
    *score_texture = SDL_CreateTextureFromSurface(renderer, score_surface);
    SDL_FreeSurface(score_surface);
}

void ResetGame(ball_t *ball, bool *game_started){
    ball->rect.x = WINDOW_W / 2;
    ball->rect.y = WINDOW_H / 2;
    ball->dx = 0;
    ball->dy = 0;
    *game_started = false;
}




int main() {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    TTF_Font* font;
    const byte* keystate;
    SDL_RendererFlags flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    
    paddle_t paddles[NUMPADDLES];
    ball_t ball;
    SDL_Texture *score_textures[2];
    SDL_Rect score_rects[2];
    bool game_started = false;
        
    score_rects[left].x = MARGIN;
    score_rects[left].y = WINDOW_H - MARGIN;
    score_rects[right].y = WINDOW_H - MARGIN;
    
    
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    window = SDL_CreateWindow("Pong", 0, 0, WINDOW_W * SCALE, WINDOW_H * SCALE, 0);
    renderer = SDL_CreateRenderer(window, -1, flags);
    SDL_RenderSetScale(renderer, SCALE, SCALE);
    
    InitSound();
    
    // init font
    if(TTF_Init() == -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(2);
    }
    
    font = TTF_OpenFont("PlantagenetCherokee.ttf", 32);
    if (!font) {
        puts("could not load font");
        exit(1);
    }
    
   
    
    keystate = SDL_GetKeyboardState(NULL);
    
    srand((unsigned)time(NULL));
    
    for(int i = 0; i < NUMPADDLES; i++) {
        // Setting initial paddle position
        paddles[i].rect.x = (i == left) ? PADDLE1_X : PADDLE2_X;
        paddles[i].rect.y = (WINDOW_H / 2) - (PADDLE_H / 2);
        paddles[i].rect.w = PADDLE_W;
        paddles[i].rect.h = PADDLE_H;
        UpdateScore(i, &score_textures[i], font, renderer, &paddles[i].score, 0, &score_rects[i]);
    }
    
    ball.rect.w = BALL_D;
    ball.rect.h = BALL_D;
    ResetGame(&ball, &game_started);
    
    while (1) {
        // Check for input
        while (SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT){
                StopSound();
                exit(0);
            }
            
            if ( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE && !game_started) {
                game_started = true;
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
        
        // Update ball position
        ball.rect.x += ball.dx;
        ball.rect.y += ball.dy;
        
        // Bounds collision detection
        if(ball.rect.x < -BALL_D){
            ResetGame(&ball, &game_started);
            
            UpdateScore(right, &score_textures[right], font, renderer, &paddles[right].score, paddles[right].score + 1, &score_rects[right]);
            PlaySound(SND_GOAL);
        }
        if(ball.rect.x > WINDOW_W){
            ResetGame(&ball, &game_started);
            UpdateScore(left, &score_textures[left], font, renderer, &paddles[left].score, paddles[left].score + 1, &score_rects[left]);
            PlaySound(SND_GOAL);
        }
        if(ball.rect.y < 0){
            ball.dy = -ball.dy;
            ball.rect.y = 0;
            PlaySound(SND_TOPHIT);
        }
        if(ball.rect.y > WINDOW_H - ball.rect.h){
            ball.dy = -ball.dy;
            ball.rect.y = WINDOW_H - ball.rect.h;
            PlaySound(SND_TOPHIT);
        }
        
        // Paddle collision detection
        if(ball.dx > 0){ // moving to the right
            if(ball.rect.y < paddles[right].rect.y + PADDLE_H &&
               ball.rect.y + BALL_D > paddles[right].rect.y &&
               ball.rect.x + BALL_D > paddles[right].rect.x)
            {
                ball.dx = -ball.dx;
                ball.rect.x = paddles[right].rect.x - BALL_D;
                PlaySound(SND_PADHIT);
            }
        } else if(ball.dx < 0) { // moving to the left
            if(ball.rect.y < paddles[left].rect.y + PADDLE_H &&
               ball.rect.y + BALL_D > paddles[left].rect.y &&
               ball.rect.x < paddles[left].rect.x + PADDLE_W)
            {
                ball.dx = -ball.dx;
                ball.rect.x = paddles[left].rect.x + PADDLE_W;
                PlaySound(SND_PADHIT);
            }
        }
        
        // DRAW EVERYTHING
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // draw scores
        for (int i = 0; i < NUMPADDLES; i++) {
            SDL_RenderCopy(renderer, score_textures[i], NULL, &score_rects[i]);
        }
        
        // draw paddles
        SDL_SetRenderDrawColor(renderer, 3, 80, 233, 255);
        SDL_RenderFillRect(renderer, &paddles[left].rect);
        SDL_RenderFillRect(renderer, &paddles[right].rect);
        
        // draw ball
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &ball.rect);
        
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }
    return 0;
}
