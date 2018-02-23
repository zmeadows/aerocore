#include "BoundingSurface.hpp"
#include "Generator.hpp"
#include "Generator/Bullet.hpp"
#include "Geometry.hpp"
#include "Globals.hpp"
#include "Entity.hpp"

UUID generateBullet(const v2& new_bullet_position, const v2& new_bullet_velocity) {
    UUID bulletUUID;

    auto CM = get_manager();

    Entity& bullet = CM->book<Entity>(bulletUUID);
    bullet.pos = new_bullet_position;
    bullet.vel = new_bullet_velocity;

    bullet.angvel = 10.f;

    bullet.color.r = 20;
    bullet.color.g = 200;
    bullet.color.b = 20;
    bullet.color.a = 200;

    bullet.friendly = true;

    assign_iso_triangle_vertices(bullet, 2.f, 1.f);
    bullet.poly_decomp = decompose_local_vertices(bullet.local_vertices);
    recompute_global_context(bulletUUID, bullet);

    play_sound(SoundEffect_ShotFired);

    return bulletUUID;
}
