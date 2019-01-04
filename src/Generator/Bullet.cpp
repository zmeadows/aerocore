#include "Generator/Bullet.hpp"
#include "Generator/Enemy.hpp"
#include "Geometry.hpp"
#include "Globals.hpp"
#include "Entity.hpp"
#include "Sprite.hpp"

UUID generate_bullet_player(const v2 new_bullet_position,
                            const v2 new_bullet_velocity,
                            float new_bullet_angle)
{
    UUID uuid;

    generate_enemy_skeleton(uuid,
                            new_bullet_position,
                            "sprites/playerbullet/playerbullet.png",
                            "sprites/playerbullet/path"
                            );

    auto CM = get_manager();
    CM->book<FriendlyTag>(uuid);

    auto& bullet = CM->get<Entity>(uuid);
    auto& kin = CM->book<EulerTranslation>(uuid);

    bullet.pos = new_bullet_position;
    bullet.angle = new_bullet_angle;
    kin.vel = new_bullet_velocity;

    play_sound(SoundEffect_ShotFired);

    return uuid;
}

UUID generate_bullet_enemy(const v2 new_bullet_position,
                           const v2 new_bullet_velocity,
                           float new_bullet_angle)
{
    UUID uuid;

    generate_enemy_skeleton(uuid,
                            new_bullet_position,
                            "sprites/playerbullet/playerbullet.png",
                            "sprites/playerbullet/path"
                            );

    auto CM = get_manager();

    auto& bullet = CM->get<Entity>(uuid);
    auto& kin = CM->book<EulerTranslation>(uuid);

    bullet.pos = new_bullet_position;
    bullet.angle = new_bullet_angle;
    kin.vel = new_bullet_velocity;

    play_sound(SoundEffect_ShotFired);

    return uuid;
}
