#include "Generator/Player.hpp"
#include "Generator/Bullet.hpp"
#include "Geometry.hpp"
#include "Globals.hpp"
#include "Entity.hpp"
#include "Sprite.hpp"

#include <random>

void generatePlayer() {
    ComponentManager* CM = get_manager();

    Entity& player = CM->book<Entity>(playerUUID());
    CM->book<EulerTranslation>(playerUUID());
    CM->book<EulerRotation>(playerUUID());
    CM->book<FriendlyTag>(playerUUID());
    CM->book<PositionUpdate>(playerUUID());
    CM->book<RotationUpdate>(playerUUID());
    auto& cd = CM->book<CollisionData>(playerUUID());
    auto& sprite = CM->book<Sprite>(playerUUID());

    player.angle = PI/2.0;

    auto parse_result = parse_svg_path(asset_path("sprites/player/path").c_str());
    cd.local_vertices = parse_result.vertices;
    sprite.offset = parse_result.sprite_offset;
    cd.poly_decomp = decompose_local_vertices(cd.local_vertices);
    populate_global_vertices(cd.local_vertices, cd.global_vertices, player.pos, player.angle);
    player.extent = extent_of(cd.global_vertices);
    cd.node = get_quad_tree()->insert_entity(playerUUID(), clip_to_screen(player.extent));
    assert(cd.node);

    sprite.image = GPU_LoadImage(asset_path("sprites/player/player.png").c_str());

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
