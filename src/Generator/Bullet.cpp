#include "Generator/Bullet.hpp"
#include "Geometry.hpp"
#include "Globals.hpp"
#include "Entity.hpp"
#include "Sprite.hpp"

UUID generateBullet(const v2 new_bullet_position, const v2 new_bullet_velocity, float new_bullet_angle) {
    UUID bulletUUID;

    auto CM = get_manager();

    Entity& bullet = CM->book<Entity>(bulletUUID);
    EulerTranslation& kin = CM->book<EulerTranslation>(bulletUUID);
    CM->book<PositionUpdate>(bulletUUID);
    CM->book<RotationUpdate>(bulletUUID);
    CM->book<OffscreenBehavior>(bulletUUID);
    auto& cd = CM->book<CollisionData>(bulletUUID);
    auto& sprite = CM->book<Sprite>(bulletUUID);

    CM->book<FriendlyTag>(bulletUUID);

    bullet.pos = new_bullet_position;
    bullet.angle = new_bullet_angle;
    kin.vel = new_bullet_velocity;

    auto parse_result = parse_svg_path(asset_path("sprites/playerbullet/path").c_str());
    cd.local_vertices = parse_result.vertices;
    sprite.offset = parse_result.sprite_offset;
    cd.poly_decomp = decompose_local_vertices(cd.local_vertices);
    populate_global_vertices(cd.local_vertices, cd.global_vertices, bullet.pos, bullet.angle);
    bullet.extent = extent_of(cd.global_vertices);
    cd.node = get_quad_tree()->insert_entity(playerUUID(), clip_to_screen(bullet.extent));

    sprite.image = GPU_LoadImage(asset_path("sprites/playerbullet/playerbullet.png").c_str());

    play_sound(SoundEffect_ShotFired);

    return bulletUUID;
}
