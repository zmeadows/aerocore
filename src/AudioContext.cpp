#include "AudioContext.hpp"

AudioContext::AudioContext(void)
{
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
    }
;
    chunks[SoundEffect_ShotFired] = Mix_LoadWAV("/Users/zac/Code/mine/aerocore-example/assets/sound/Laser_Shoot14.wav");
    chunks[SoundEffect_Explosion] = Mix_LoadWAV("/Users/zac/Code/mine/aerocore-example/assets/sound/Explosion19.wav");
}

AudioContext::~AudioContext(void)
{
    for (size_t i = 0; i < NUM_SOUND_EFFECTS; i++)
        Mix_FreeChunk(chunks[i]);

    Mix_CloseAudio();
}
