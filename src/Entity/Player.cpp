#include "Entity/Player.hpp"
#include "Bullet/Bullet.hpp"
#include "Geometry.hpp"
#include "Globals.hpp"

#include "Component/Common.hpp"
#include "Component/OffscreenBehavior.hpp"

const UUID playerUUID(void) {
    static UUID id;
    return id;
}

void generate_player() {
    ComponentManager* CM = get_manager();

    Entity& player = CM->book<Entity>(playerUUID());
    player.angle = PI/2.0;

    CM->book<EulerTranslation>(playerUUID());
    CM->book<EulerRotation>(playerUUID());
    CM->book<FriendlyTag>(playerUUID());

    get_sprite_cache()->attach_sprite_to_uuid(playerUUID(), "player_idle");

    auto& osb = CM->book<OffscreenBehavior>(playerUUID());
    osb.type = OffscreenBehavior::Type::Wraps;
}

void generate_player_bullet(v2 bullet_direction) {
    auto CM = get_manager();

    bullet_direction = bullet_direction.normalized();

    const auto& entity = CM->get<Entity>(playerUUID());
    const v2 bullet_pos = entity.pos + 4.0 * bullet_direction;
    const v2 bullet_vel = 200.f * bullet_direction;

    generate_bullet(PLAYER_BULLET, bullet_pos, bullet_vel, true);
}

