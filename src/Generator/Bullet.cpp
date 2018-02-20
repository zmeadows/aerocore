#include "BoundingSurface.hpp"
#include "Generator.hpp"
#include "Generator/Bullet.hpp"
#include "Geometry.hpp"
#include "Globals.hpp"

UUID generateBullet(void) {
    UUID bulletUUID;

    auto CM = get_manager();

    Entity& entity = CM->book<Entity>(bulletUUID);

    entity.angvel = 10.f;

    entity.color.r = 20;
    entity.color.g = 200;
    entity.color.b = 20;
    entity.color.a = 200;

    entity.vertices = make_square_vertices(0.75);

    auto& coldat = CM->book<CollisionData>(bulletUUID);
    coldat.friendly = true;
    coldat.triangles = decompose(entity.vertices);
    coldat.normals = SurfaceNormalSet(coldat.triangles);
    coldat.node = get_quad_tree()->insert_entity(bulletUUID, extent_of(entity));

    return bulletUUID;
}
