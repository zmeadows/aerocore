#include "AudioContext.hpp"

#include "Util.hpp"

AudioContext::AudioContext(void)
{
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
    }

    chunks[SoundEffect_ShotFired] = Mix_LoadWAV(asset_path("sound/Laser_Shoot14.wav").c_str());
    chunks[SoundEffect_Explosion] = Mix_LoadWAV(asset_path("sound/Explosion19.wav").c_str());

    for (size_t i = 0; i < NUM_SOUND_EFFECTS; i++)
        Mix_VolumeChunk(chunks[i], MIX_MAX_VOLUME/20);
}

AudioContext::~AudioContext(void)
{
    for (size_t i = 0; i < NUM_SOUND_EFFECTS; i++)
        Mix_FreeChunk(chunks[i]);

    Mix_CloseAudio();
}
