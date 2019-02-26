#include "Bullet/Bullet.hpp"

#include "AudioContext.hpp"
#include "Component/Common.hpp"
#include "Component/OffscreenBehavior.hpp"
#include "Engine/ComponentManager.hpp"
#include "Geometry.hpp"
#include "SpriteCache.hpp"

UUID generate_bullet(ComponentManager* CM,
                     AudioContext* AC,
                     SpriteCache* SC,
                     BulletType bullet_type,
                     const v2 new_bullet_position,
                     const v2 new_bullet_velocity,
                     bool friendly)
{
    UUID uuid;

    auto& entity = CM->book<Entity>(uuid);
    entity.pos = new_bullet_position;
    entity.angle = new_bullet_velocity.angle();

    CM->book<EulerTranslation>(uuid).vel = new_bullet_velocity;
    CM->book<OffscreenBehavior>(uuid);
    CM->book<CollideDamage>(uuid);
    CM->book<BulletTag>(uuid);

    if (friendly) CM->book<FriendlyTag>(uuid);

    switch (bullet_type) {
        case PLAYER_BULLET: {
            SC->attach_sprite_to_uuid(CM, uuid, "bullet_player");
            AC->play(SoundEffect_ShotFired);
            break;
        }

        case ENEMY_BULLET: {
            SC->attach_sprite_to_uuid(CM, uuid, "bullet_enemy");
            break;
        }
    }

    return uuid;
}

