#include "Globals.hpp"
#include "Generator.hpp"
#include "Geometry.hpp"
#include "Generator/Enemy.hpp"
#include "Entity.hpp"

UUID generateOffscreenAsteroid(void) {
    UUID enemyUUID;
    auto CM = get_manager();

    Entity& asteroid = CM->book<Entity>(enemyUUID);

    asteroid.type = EntityType::Asteroid;

    asteroid.color.r = 200;
    asteroid.color.g = 20;
    asteroid.color.b = 20;
    asteroid.color.a = 200;

    asteroid.local_vertices = {
        {-15.f + uniform_rand(-5.0,5.0), -15.f + uniform_rand(-5.0,5.0)} ,
        {-7.f  + uniform_rand(-5.0,5.0), 0.f   + uniform_rand(-5.0,5.0)} ,
        {-15.f + uniform_rand(-5.0,5.0), 15.f  + uniform_rand(-5.0,5.0)} ,
        {0.f   + uniform_rand(-5.0,5.0), 25.f  + uniform_rand(-5.0,5.0)} ,
        {15.f  + uniform_rand(-5.0,5.0), 15.f  + uniform_rand(-5.0,5.0)} ,
        {7.f   + uniform_rand(-5.0,5.0), 0.f   + uniform_rand(-5.0,5.0)} ,
        {15.f  + uniform_rand(-5.0,5.0), -15.f + uniform_rand(-5.0,5.0)} ,
    };

    // initially place the asteroid in a random location just off screen

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

    asteroid.pos = random_position;

    asteroid.angle = uniform_rand(0.f, TWOPI - 0.001f);

    const float sf = uniform_rand(0.4f,1.2f);
    for (v2& vtx : asteroid.local_vertices) {
        vtx.scale(sf);
    };

    // aim the asteroid roughly towards the center of the screen

    v2 rough_origin;
    rough_origin.x = uniform_rand(-50.f, 50.f);
    rough_origin.y = uniform_rand(-50.f, 50.f);

    v2 random_velocity = (rough_origin - random_position).normalized();
    random_velocity.scale(uniform_rand(10,30));
    asteroid.vel = random_velocity;

    // random but slow rotation
    asteroid.angvel = uniform_rand(-0.5, 0.5);

    reset_global_vertices(asteroid);
    asteroid.extent = extent_of(asteroid);

    auto& coldat = CM->book<CollisionData>(enemyUUID, asteroid);
    coldat.friendly = false;
    coldat.node = get_quad_tree()->insert_entity(enemyUUID, asteroid.extent);

    auto& osb = CM->book<OffscreenBehavior>(enemyUUID);
    osb = SinglePassOSB();

    return enemyUUID;
}

void generateAsteroidShards(const Entity& parent_asteroid,
                            const std::vector<std::vector<size_t>>& triangle_indices)
{
    assert(parent_asteroid.type == EntityType::Asteroid);

    const v2 asteroid_center = average_vec(parent_asteroid.global_vertices);

    auto CM = get_manager();

    for (const std::vector<size_t>& triangle : triangle_indices) {
        UUID shardUUID;

        Entity& shard = CM->book<Entity>(shardUUID);

        shard.pos = parent_asteroid.pos;
        shard.angle = parent_asteroid.angle;
        shard.color = parent_asteroid.color;
        shard.local_vertices.resize(3);

        size_t i = 0;
        for (size_t idx : triangle) {
            shard.local_vertices[i] = parent_asteroid.local_vertices[idx];
            i++;
        }
        reset_global_vertices(shard);
        shard.extent = extent_of(shard);

        // shoot the shard away from the asteroid center
        const v2 shard_center = average_vec(shard.global_vertices);
        v2 vel_offset = (shard_center - asteroid_center).normalized();
        vel_offset.scale(10.f);
        shard.vel = parent_asteroid.vel + vel_offset;

        shard.acc = parent_asteroid.acc;
        shard.angvel = parent_asteroid.angvel + 0.5f * uniform_rand(-3.f,3.f);
        shard.type = EntityType::Effect;

        CM->book<AsteroidShardData>(shardUUID, 1.f);
    }
}

// updates shard state. returns alpha of current shard
Uint8 shard_tick(AsteroidShardData& shard, float dt) {
    shard.time_left -= dt;
    if (shard.time_left <= 0.f) {
        shard.time_left = 0.f;
        return 0;
    } else {
        assert( shard.time_left / shard.lifetime <= 1.f );
        assert( shard.time_left / shard.lifetime >= 0.f );
        return static_cast<Uint8>(nearbyint(255.f * std::pow(shard.time_left / shard.lifetime, 2.f)));
    }
};

void generateStabber(void) {
    UUID uuid;
    auto CM = get_manager();

    Entity& entity = CM->book<Entity>(uuid);

    entity.pos = { uniform_rand(-80.f, 80.f), uniform_rand(-80.f, 80.f) };

    entity.local_vertices = {
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

    reset_global_vertices(entity);
    entity.extent = extent_of(entity);

    entity.color = { 15, 189, 73, 255 };
    entity.type = EntityType::Stabber;

    auto& coldat = CM->book<CollisionData>(uuid, entity);
    coldat.friendly = false;
    coldat.node = get_quad_tree()->insert_entity(uuid, entity.extent);

    auto& stabber = CM->book<StabberData>(uuid);
    set_stabber_to_relocating(entity, stabber);
}

void set_stabber_to_relocating(Entity& stabber, StabberData& stabber_data)
{
    stabber_data.idle_point = { uniform_rand(-80.f, 80.f), uniform_rand(-80.f, 80.f) };

    stabber_data.time_since_last_stab = 0.f;

    stabber.acc = (stabber_data.idle_point - stabber.pos).normalized();
    stabber.acc.scale(6.f);

    stabber_data.state = StabberData::State::Relocating;
}
