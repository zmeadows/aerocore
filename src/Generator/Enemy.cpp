#include "Generator.hpp"
#include "Geometry.hpp"
#include "Generator/Enemy.hpp"

UUID generateOffscreenAsteroid(ComponentManager* CM, QuadTree* quadTree) {
    UUID enemyUUID;
    CoreData& CD = CM->book<CoreData>(enemyUUID);

    v2 random_position;

    float r = uniform_rand(0.0, 1.0);
    r = uniform_rand(0.0, 1.0);
    if (r < 0.25) {
        random_position.x = uniform_rand(-200, -120);
        random_position.y = uniform_rand(-200, -120);
    } else if (r < 0.5) {
        random_position.x = uniform_rand(120, 200);
        random_position.y = uniform_rand(-200, -120);
    } else if (r < 0.75) {
        random_position.x = uniform_rand(-200, -120);
        random_position.y = uniform_rand(120, 200);
    } else if (r < 1.0) {
        random_position.x = uniform_rand(120, 200);
        random_position.y = uniform_rand(120, 200);
    }

    v2 rough_origin;
    rough_origin.x = uniform_rand(-90, 90);
    rough_origin.y = uniform_rand(-90, 90);

    CD.pos = random_position;
    v2 random_velocity = (rough_origin - random_position).normalized();
    random_velocity.scale(uniform_rand(10,30));
    CD.vel = random_velocity;

    float sf = uniform_rand(0.5,1.2);
    for (v2& vtx : CD.vertices) {
        vtx.scale(sf);
    };

    CD.color.r = 200;
    CD.color.g = 20;
    CD.color.b = 20;
    CD.color.a = 200;

    CD.vertices = {
        {-15.f + uniform_rand(-5.0,5.0), -15.f + uniform_rand(-5.0,5.0)} ,
        {-7.f  + uniform_rand(-5.0,5.0), 0.f   + uniform_rand(-5.0,5.0)} ,
        {-15.f + uniform_rand(-5.0,5.0), 15.f  + uniform_rand(-5.0,5.0)} ,
        {0.f   + uniform_rand(-5.0,5.0), 25.f  + uniform_rand(-5.0,5.0)} ,
        {15.f  + uniform_rand(-5.0,5.0), 15.f  + uniform_rand(-5.0,5.0)} ,
        {7.f   + uniform_rand(-5.0,5.0), 0.f   + uniform_rand(-5.0,5.0)} ,
        {15.f  + uniform_rand(-5.0,5.0), -15.f + uniform_rand(-5.0,5.0)} ,
    };

    auto& coldat = CM->book<CollisionData>(enemyUUID);
    coldat.friendly = false;
    coldat.triangles = triangulate(CD.vertices);
    coldat.normals = SurfaceNormalSet(coldat.triangles);
    coldat.node = quadTree->insert_entity(enemyUUID, extent_at(CD));

    auto& osb = CM->book<OffscreenBehavior>(enemyUUID);
    osb = SinglePassOSB();

    CD.angvel = uniform_rand(-0.5, 0.5);

    CD.wraps = false;


    return enemyUUID;
}
