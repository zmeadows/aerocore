#pragma once

#include "unstd/types.hpp"

struct v2;
class UUID;
class ComponentManager;
class AudioContext;
class SpriteCache;
class PlayerInput;

const UUID playerUUID(void);

struct PlayerState {
    f32 shot_countdown = 0.f;
    f32 shot_delay = 0.2f;
};

void generate_player(ComponentManager* CM, SpriteCache* SC);

void generate_player_bullet(ComponentManager* CM,
                            AudioContext* AC,
                            SpriteCache* SC,
                            v2 bullet_direction);

void tick_player(ComponentManager* CM,
                 AudioContext* AC,
                 SpriteCache* SC,
                 PlayerInput* input,
                 PlayerState* state,
                 f32 dt);
