#include "Generator.hpp"
#include "Generator/Bullet.hpp"
#include "Geometry.hpp"
#include "BoundingSurface.hpp"

UUID generateBullet(ComponentManager* CM, QuadTree* quadTree) {
    UUID bulletUUID;

    CoreData& CD = CM->book<CoreData>(bulletUUID);

    CD.angvel = 10.f;

    CD.color.r = 20;
    CD.color.g = 200;
    CD.color.b = 20;
    CD.color.a = 200;

    CD.vertices = make_square_vertices(0.75);

    auto& coldat = CM->book<CollisionData>(bulletUUID);
    coldat.friendly = true;
    coldat.triangles = triangulate(CD.vertices);
    coldat.normals = SurfaceNormalSet(coldat.triangles);
    coldat.node = quadTree->insert_entity(bulletUUID, extent_at(CD));

    return bulletUUID;
}
