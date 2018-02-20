#include "Globals.hpp"
#include "Generator.hpp"
#include "Geometry.hpp"
#include "Generator/Enemy.hpp"

UUID generateOffscreenAsteroid(void) {
    UUID enemyUUID;
    auto CM = get_manager();

    Entity& entity = CM->book<Entity>(enemyUUID);

    entity.type = EntityType::Asteroid;

    v2 random_position;

    float r = uniform_rand(0.f, 1.f);
    r = uniform_rand(0.f, 1.f);
    if (r < 0.25f) {
        random_position.x = uniform_rand(-200.f, -120.f);
        random_position.y = uniform_rand(-200.f, -120.f);
    } else if (r < 0.5f) {
        random_position.x = uniform_rand(120.f, 200.f);
        random_position.y = uniform_rand(-200.f, -120.f);
    } else if (r < 0.75f) {
        random_position.x = uniform_rand(-200.f, -120.f);
        random_position.y = uniform_rand(120.f, 200.f);
    } else if (r < 1.f) {
        random_position.x = uniform_rand(120.f, 200.f);
        random_position.y = uniform_rand(120.f, 200.f);
    }

    v2 rough_origin;
    rough_origin.x = uniform_rand(-90.f, 90.f);
    rough_origin.y = uniform_rand(-90.f, 90.f);

    entity.pos = random_position;
    v2 random_velocity = (rough_origin - random_position).normalized();
    random_velocity.scale(uniform_rand(10,30));
    entity.vel = random_velocity;

    float sf = uniform_rand(0.5f,1.2f);
    for (v2& vtx : entity.vertices) {
        vtx.scale(sf);
    };

    entity.color.r = 200;
    entity.color.g = 20;
    entity.color.b = 20;
    entity.color.a = 200;

    entity.vertices = {
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
    coldat.triangles = decompose(entity.vertices);
    coldat.normals = SurfaceNormalSet(coldat.triangles);
    coldat.node = get_quad_tree()->insert_entity(enemyUUID, extent_of(entity));

    auto& osb = CM->book<OffscreenBehavior>(enemyUUID);
    osb = SinglePassOSB();

    entity.angvel = uniform_rand(-0.5, 0.5);

    return enemyUUID;
}

void generateAsteroidShards(const Entity& asteroid,
                            const std::vector<std::vector<v2>>& triangles)
{
    assert(asteroid.type == EntityType::Asteroid);

    const v2 asteroid_center = average_vec(transform_vertices(asteroid));

    auto CM = get_manager();

    for (const std::vector<v2>& tri : triangles) {
        UUID shardUUID;

        Entity& CD = CM->book<Entity>(shardUUID);

        CD.vertices = tri;
        CD.color = asteroid.color;
        CD.pos = asteroid.pos;
        CD.vel = asteroid.vel;
        CD.acc = asteroid.acc;
        CD.angle = asteroid.angle;
        CD.angvel = asteroid.angvel + 0.3 * uniform_rand(-3.f,3.f);
        CD.type = EntityType::Effect;

        const v2 shard_center = average_vec(transform_vertices(CD));
        v2 vel_offset = (shard_center - asteroid_center).normalized();
        vel_offset.scale(10.f);
        CD.vel = CD.vel + vel_offset;

        auto& shard = CM->book<AsteroidShardData>(shardUUID, 1.f);
    }
}

// updates shard state. returns alpha of current shard
Uint8 shard_tick(AsteroidShardData& shard, float dt) {
    shard.time_left -= dt;
    if (shard.time_left <= 0.f) {
        shard.time_left = 0.f;
        return 0;
    } else {
        return static_cast<unsigned>(nearbyint(255.f * std::pow(shard.time_left / shard.lifetime, 2.f)));
    }
};

void generateStabber(void) {
    UUID uuid;
    auto CM = get_manager();

    Entity& entity = CM->book<Entity>(uuid);

    entity.vertices = {
        { -1.f, -2.f },
        { -2.f, -1.f },
        { -2.f, 0.f },
        { -1.f, 3.f },
        { -1.f, 0.f },
        { 1.f, 0.f },
        { 1.f, 3.f },
        { 2.f, 0.f },
        { 2.f, -1.f },
        { 1.f, -2.f }
    };

    entity.color = { 15, 189, 73, 255 };
    entity.type = EntityType::Stabber;

    auto& coldat = CM->book<CollisionData>(uuid);
    coldat.friendly = false;
    coldat.triangles = decompose(entity.vertices);
    coldat.normals = SurfaceNormalSet(coldat.triangles);
    coldat.node = get_quad_tree()->insert_entity(uuid, extent_of(entity));

    entity.pos = { 50.f, 50.f };

    auto& stabber = CM->book<StabberData>(uuid);
    set_stabber_to_relocating(entity, stabber);
}

void set_stabber_to_relocating(Entity& stabber, StabberData& stabber_data)
{
    stabber_data.idle_point = { uniform_rand(-80.f, 80.f), uniform_rand(-80.f, 80.f) };

    std::cout << stabber_data.idle_point.x << " " << stabber_data.idle_point.y << std::endl;

    stabber_data.time_since_last_stab = 0.f;

    stabber.acc = (stabber_data.idle_point - stabber.pos).normalized();
    stabber.acc.scale(6.f);

    stabber_data.state = StabberData::State::Relocating;
}
