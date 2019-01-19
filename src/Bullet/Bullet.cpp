#include "Bullet/Bullet.hpp"
#include "Geometry.hpp"
#include "Globals.hpp"

#include "Component/Common.hpp"
#include "Component/OffscreenBehavior.hpp"

UUID generate_bullet(BulletType bullet_type,
                     const v2 new_bullet_position,
                     const v2 new_bullet_velocity,
                     bool friendly)
{
    auto CM = get_manager();

    UUID uuid;

    auto& entity = CM->book<Entity>(uuid);
    entity.pos = new_bullet_position;
    entity.angle = new_bullet_velocity.angle();

    CM->book<EulerTranslation>(uuid).vel = new_bullet_velocity;
    CM->book<OffscreenBehavior>(uuid);

    if (friendly) CM->book<FriendlyTag>(uuid);

    switch (bullet_type) {
        case PLAYER_BULLET: {
            get_sprite_cache()->attach_sprite_to_uuid(uuid, "bullet_player");
            get_audio_context()->play(SoundEffect_ShotFired);
            break;
        }

        case ENEMY_BULLET: {
            get_sprite_cache()->attach_sprite_to_uuid(uuid, "bullet_enemy");
            break;
        }
    }

    return uuid;
}

