#pragma once

struct v2;
class ComponentManager;
class UUID;
class AudioContext;
class SpriteCache;

enum BulletType {
    PLAYER_BULLET,
    ENEMY_BULLET
};

UUID generate_bullet(ComponentManager* CM,
                     AudioContext* AC,
                     SpriteCache* SC,
                     BulletType bullet_type,
                     const v2 new_bullet_position,
                     const v2 new_bullet_velocity,
                     bool friendly);


