#pragma once

#include "Typedef.hpp"

struct v2;
class UUID;

const UUID playerUUID(void);

struct PlayerState {
    f32 shot_countdown = 0.f;
    f32 shot_delay = 0.2f;
};

void generate_player(void);
void generate_player_bullet(v2 bullet_direction);
