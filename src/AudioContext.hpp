#pragma once

#include <SDL.h>
#include <SDL_mixer.h>

enum SoundEffect {
    SoundEffect_ShotFired = 0,
    SoundEffect_Explosion,
    NUM_SOUND_EFFECTS
};

struct SoundEvent {
    SoundEvent(SoundEffect _effect) : effect(_effect) {}
    const enum SoundEffect effect;
};

class AudioContext {
    Mix_Chunk* chunks[NUM_SOUND_EFFECTS];
public:

    AudioContext(void);
    ~AudioContext(void);

    AudioContext(const AudioContext&)            = delete;
    AudioContext& operator=(const AudioContext&) = delete;
    AudioContext(AudioContext&&)                 = delete;
    AudioContext& operator=(AudioContext&&)      = delete;

    void play(SoundEffect effect) { Mix_PlayChannel(-1, chunks[effect], 0); }
};

