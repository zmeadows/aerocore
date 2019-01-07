#include "Globals.hpp"
#include "Geometry.hpp"
#include "Generator/Enemy.hpp"
#include "Entity.hpp"
#include "Sprite.hpp"

#include <memory>
#include <unordered_map>

// void generateStabber(void) {
//     UUID uuid;
//     auto CM = get_manager();
//
//     Entity& entity = CM->book<Entity>(uuid);
//     CM->book<EulerTranslation>(uuid);
//     CM->book<EulerRotation>(uuid);
//     CM->book<PositionUpdate>(uuid);
//     CM->book<RotationUpdate>(uuid);
//     auto& cd = CM->book<CollisionData>(uuid);
//     auto& sprite = CM->book<Sprite>(uuid);
//
//     entity.pos = { uniform_rand(-80.f, 80.f), uniform_rand(-80.f, 80.f) };
//
//     auto parse_result = parse_svg_path(asset_path("sprites/stabber/path").c_str());
//     cd.local_vertices = parse_result.vertices;
//     sprite.offset = parse_result.sprite_offset;
//     auto stsprite = GPU_LoadImage(asset_path("sprites/stabber/stabber.png").c_str());
//     sprite.image = stsprite;
//
//     entity.angle = uniform_rand(0.f, PI/2.f - 1e-3f);
//     populate_global_vertices(cd.local_vertices, cd.global_vertices, entity.pos, entity.angle);
//     entity.extent = extent_of(cd.global_vertices);
//     cd.poly_decomp = decompose_local_vertices(cd.local_vertices);
//     cd.node = get_quad_tree()->insert_entity(playerUUID(), clip_to_screen(entity.extent));
//
//     auto& stabber = CM->book<Stabber>(uuid);
//     stabber.speed = uniform_rand(75,100);
// }

namespace {

struct ImagePtrDeleter {
    void operator()(GPU_Image* p) { GPU_FreeImage(p); }
};

using ImagePtr = std::unique_ptr<GPU_Image, ImagePtrDeleter>;

std::unordered_map<std::string, ImagePtr> sprite_cache;

GPU_Image* make_sprite(const std::string& image_path) {
    auto it = sprite_cache.find(image_path);
    if (it != sprite_cache.end()) {
        return it->second.get();
    } else {
        auto image = GPU_LoadImage(image_path.c_str());
        sprite_cache.emplace(image_path, image);
        return image;
    }
}

}

//TODO: define separate 'entity skeleton' classes and do multiple dispatch here on them
void generate_enemy_skeleton(UUID uuid, v2 position, const char* sprite_file, const char* path_file) {
    auto CM = get_manager();

    Entity& entity = CM->book<Entity>(uuid);
    entity.pos = position;
    entity.angle = 0.f;
    CM->book<PositionUpdate>(uuid);
    CM->book<RotationUpdate>(uuid);
    auto& cd = CM->book<CollisionData>(uuid);
    auto& sprite = CM->book<Sprite>(uuid);

    auto& osb = CM->book<OffscreenBehavior>(uuid);

    auto parse_result = parse_svg_path(asset_path(path_file).c_str());
    cd.local_vertices = parse_result.vertices;
    sprite.offset = parse_result.sprite_offset;
    sprite.image = make_sprite(asset_path(sprite_file));

    // populate_global_vertices(cd.local_vertices, cd.global_vertices, entity.pos, entity.angle);
    entity.extent = extent_of(cd.local_vertices, entity.pos, entity.angle);
    cd.poly_decomp = decompose_local_vertices(cd.local_vertices);
    cd.node = get_quad_tree()->insert_entity(uuid, clip_to_screen(entity.extent));

}
