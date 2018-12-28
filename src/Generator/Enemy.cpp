#include "Globals.hpp"
#include "Geometry.hpp"
#include "Generator/Enemy.hpp"
#include "Entity.hpp"
#include "Sprite.hpp"

// UUID generateOffscreenAsteroid(void) {
//     UUID enemyUUID;
//     auto CM = get_manager();
//
//     Entity& asteroid = CM->book<Entity>(enemyUUID);
//
//     asteroid.local_vertices[0] = {-15.f + uniform_rand(-5.0,5.0), -15.f + uniform_rand(-5.0,5.0)};
//     asteroid.local_vertices[1] = {-7.f  + uniform_rand(-5.0,5.0), 0.f   + uniform_rand(-5.0,5.0)};
//     asteroid.local_vertices[2] = {-15.f + uniform_rand(-5.0,5.0), 15.f  + uniform_rand(-5.0,5.0)};
//     asteroid.local_vertices[3] = {0.f   + uniform_rand(-5.0,5.0), 25.f  + uniform_rand(-5.0,5.0)};
//     asteroid.local_vertices[4] = {15.f  + uniform_rand(-5.0,5.0), 15.f  + uniform_rand(-5.0,5.0)};
//     asteroid.local_vertices[5] = {7.f   + uniform_rand(-5.0,5.0), 0.f   + uniform_rand(-5.0,5.0)};
//     asteroid.local_vertices[6] = {15.f  + uniform_rand(-5.0,5.0), -15.f + uniform_rand(-5.0,5.0)};
//     asteroid.local_vertices.count = 7;
//
//     // initially place the asteroid in a random location just off screen
//
//     v2 random_position;
//
//     float r = uniform_rand(0.f, 1.f);
//     r = uniform_rand(0.f, 1.f);
//     if (r < 0.25f) {
//         random_position.x = uniform_rand(-200.f, -120.f);
//         random_position.y = uniform_rand(-200.f, -120.f);
//     } else if (r < 0.5f) {
//         random_position.x = uniform_rand(120.f, 200.f);
//         random_position.y = uniform_rand(-200.f, -120.f);
//     } else if (r < 0.75f) {
//         random_position.x = uniform_rand(-200.f, -120.f);
//         random_position.y = uniform_rand(120.f, 200.f);
//     } else if (r < 1.f) {
//         random_position.x = uniform_rand(120.f, 200.f);
//         random_position.y = uniform_rand(120.f, 200.f);
//     }
//
//     asteroid.pos = random_position;
//
//     asteroid.angle = uniform_rand(0.f, TWOPI - 0.001f);
//
//     const float sf = uniform_rand(0.4f,1.2f);
//     for (v2& vtx : asteroid.local_vertices) {
//         vtx.scale(sf);
//     };
//
//     // aim the asteroid roughly towards the center of the screen
//     v2 rough_origin;
//     rough_origin.x = uniform_rand(-50.f, 50.f);
//     rough_origin.y = uniform_rand(-50.f, 50.f);
//
//     v2 random_velocity = (rough_origin - random_position).normalized();
//     random_velocity.scale(uniform_rand(10,30));
//     asteroid.vel = random_velocity;
//
//     // random but slow rotation
//     // asteroid.angvel = uniform_rand(-0.5, 0.5);
//     asteroid.friendly = false;
//
//     recompute_global_context(enemyUUID, asteroid);
//     asteroid.poly_decomp = decompose_local_vertices(asteroid.local_vertices);
//
//     // asteroid.osb.type = OffscreenBehavior::Type::SinglePassAllowed;
//     // asteroid.osb.SinglePassAllowed.already_found_onscreen = false;
//
//     return enemyUUID;
// }

/*
void generateAsteroidShards(const Entity& parent_asteroid,
                            const std::vector<std::vector<size_t>>& triangle_indices)
{
    assert(parent_asteroid.type == EntityType::Asteroid);

    const v2 asteroid_center = average_vector(parent_asteroid.global_vertices,
                                              parent_asteroid.vertex_count);

    auto CM = get_manager();

    for (const std::vector<size_t>& triangle : triangle_indices) {
        UUID shardUUID;

        Entity& shard = CM->book<Entity>(shardUUID);

        shard.pos = parent_asteroid.pos;
        shard.angle = parent_asteroid.angle;
        shard.color = parent_asteroid.color;

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
*/

// updates shard state. returns alpha of current shard
// Uint8 shard_tick(AsteroidShardData& shard, float dt) {
//     shard.time_left -= dt;
//     if (shard.time_left <= 0.f) {
//         shard.time_left = 0.f;
//         return 0;
//     } else {
//         assert( shard.time_left / shard.lifetime <= 1.f );
//         assert( shard.time_left / shard.lifetime >= 0.f );
//         return static_cast<Uint8>(nearbyint(255.f * std::pow(shard.time_left / shard.lifetime, 2.f)));
//     }
// };

void generateStabber(void) {
    UUID uuid;
    auto CM = get_manager();

    Entity& entity = CM->book<Entity>(uuid);
    CM->book<EulerTranslation>(uuid);
    CM->book<EulerRotation>(uuid);
    CM->book<PositionUpdate>(uuid);
    CM->book<RotationUpdate>(uuid);
    auto& cd = CM->book<CollisionData>(uuid);
    auto& sprite = CM->book<Sprite>(uuid);

    entity.pos = { uniform_rand(-80.f, 80.f), uniform_rand(-80.f, 80.f) };

    auto parse_result = parse_svg_path(asset_path("sprites/stabber/path").c_str());
    cd.local_vertices = parse_result.vertices;
    sprite.offset = parse_result.sprite_offset;
    static auto stsprite = GPU_LoadImage(asset_path("sprites/stabber/stabber.png").c_str());
    sprite.image = stsprite;

    entity.angle = uniform_rand(0.f, PI/2.f - 1e-3f);
    populate_global_vertices(cd.local_vertices, cd.global_vertices, entity.pos, entity.angle);
    entity.extent = extent_of(cd.global_vertices);
    cd.poly_decomp = decompose_local_vertices(cd.local_vertices);
    cd.node = get_quad_tree()->insert_entity(playerUUID(), clip_to_screen(entity.extent));

    auto& stabber = CM->book<Stabber>(uuid);
    stabber.speed = uniform_rand(75,100);
}

void set_stabber_to_relocating(Entity& stabber, Stabber& stabber_data)
{
}
