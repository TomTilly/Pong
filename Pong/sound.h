//
//  sound.h
//  Pong
//
//  Created by Thomas Tillistrand on 11/1/19.
//  Copyright © 2019 Thomas Tillistrand. All rights reserved.
//

#ifndef sound_h
#define sound_h

typedef enum {
    SND_PADHIT,
    SND_TOPHIT,
    SND_GOAL,
    SND_COUNT
} sound_t;

void StopSound(void);
void InitSound(void);
void PlaySound(sound_t sound);

#endif /* sound_h */
