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

    bullet.local_vertices = make_square_vertices(0.75);
    reset_global_vertices(bullet);
    bullet.extent = extent_of(bullet);

    auto& coldat = CM->book<CollisionData>(bulletUUID, bullet);
    coldat.friendly = true;
    coldat.node = get_quad_tree()->insert_entity(bulletUUID, bullet.extent);

    return bulletUUID;
}
