#include "Bullet/Bullet.hpp"
#include "Geometry.hpp"
#include "Globals.hpp"
#include "Entity.hpp"

UUID generate_bullet_player(const v2 new_bullet_position,
                            const v2 new_bullet_velocity,
                            float new_bullet_angle)
{
    auto CM = get_manager();

    UUID uuid;

    auto& entity = CM->book<Entity>(uuid);
    entity.pos = new_bullet_position;
    entity.angle = new_bullet_angle;
    CM->book<EulerTranslation>(uuid).vel = new_bullet_velocity;
    CM->book<OffscreenBehavior>(uuid);

    get_sprite_cache()->attach_sprite_to_uuid(uuid, "bullet_player");

    CM->book<FriendlyTag>(uuid);

    play_sound(SoundEffect_ShotFired);

    return uuid;
}

UUID generate_bullet_enemy(const v2 new_bullet_position,
                           const v2 new_bullet_velocity,
                           float new_bullet_angle)
{
    auto CM = get_manager();

    UUID uuid;

    auto& entity = CM->book<Entity>(uuid);
    entity.pos = new_bullet_position;
    entity.angle = new_bullet_angle;

    CM->book<EulerTranslation>(uuid).vel = new_bullet_velocity;
    CM->book<OffscreenBehavior>(uuid);

    get_sprite_cache()->attach_sprite_to_uuid(uuid, "bullet_enemy");

    return uuid;
}
