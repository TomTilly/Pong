//
//  sound.c
//  Pong
//
//  Created by Thomas Tillistrand on 11/1/19.
//  Copyright © 2019 Thomas Tillistrand. All rights reserved.
//

#include "sound.h"
#include <SDL2_mixer/SDL_mixer.h>

#define VOLUME 8

Mix_Chunk *sounds[SND_COUNT];

void InitSound(){
    if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 512) == -1){
        printf("Error: Mix_OpenAudio failed");
    }
    
    sounds[SND_PADHIT] = Mix_LoadWAV("padhit.wav");
    sounds[SND_TOPHIT] = Mix_LoadWAV("tophit.wav");
    sounds[SND_GOAL] = Mix_LoadWAV("goal.wav");
    
    for (int i = 0; i < SND_COUNT; i++){
        Mix_VolumeChunk(sounds[i], VOLUME);
    }
}

void StopSound(){
    Mix_CloseAudio();
    for (int i = 0; i < SND_COUNT; i++) {
        Mix_FreeChunk(sounds[i]);
    }
}

void PlaySound(sound_t sound){
    Mix_PlayChannel(-1, sounds[sound], 0);
}
