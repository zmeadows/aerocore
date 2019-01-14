#include "Entity/Player.hpp"
#include "Bullet/Bullet.hpp"
#include "Geometry.hpp"
#include "Globals.hpp"
#include "Entity.hpp"

#include <random>

void generatePlayer() {
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

void generatePlayerBullet(void) {
    auto CM = get_manager();

    const auto& player = CM->get<Entity>(playerUUID());
    const v2& player_pos = player.pos;
    const v2& player_orientation = orientation_of(player);

    v2 bullet_pos, bullet_vel;

    v2 bullet_pos_offset = 4.0 * player_orientation;

    bullet_pos = player_pos + bullet_pos_offset;
    bullet_vel = 200.f * orientation_of(player);

    generate_bullet_player(bullet_pos, bullet_vel, player.angle);
}
