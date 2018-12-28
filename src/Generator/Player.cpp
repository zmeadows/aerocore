#include "Generator/Player.hpp"
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

    auto parse_result = parse_svg_path(asset_path("sprites/player/path").c_str());
    cd.local_vertices = parse_result.vertices;
    sprite.offset = parse_result.sprite_offset;
    cd.poly_decomp = decompose_local_vertices(cd.local_vertices);
    populate_global_vertices(cd.local_vertices, cd.global_vertices, player.pos, player.angle);
    player.extent = extent_of(cd.global_vertices);
    cd.node = get_quad_tree()->insert_entity(playerUUID(), clip_to_screen(player.extent));
    assert(player.node);

    sprite.image = GPU_LoadImage(asset_path("sprites/player/player.png").c_str());

    auto& osb = CM->book<OffscreenBehavior>(playerUUID());
    osb.type = OffscreenBehavior::Type::Wraps;
}
