#pragma once

#include "Globals.hpp"

struct v2;
class UUID;

enum BulletType {
    PLAYER_BULLET,
    ENEMY_BULLET
};

UUID generate_bullet(BulletType bullet_type,
                     const v2 new_bullet_position,
                     const v2 new_bullet_velocity,
                     bool friendly);


