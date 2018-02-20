#pragma once

#include "QuadTree.hpp"

UUID generateOffscreenAsteroid(void);

void generateAsteroidShards(const Entity& asteroid,
                            const std::vector<std::vector<size_t>>& triangles_indices);

struct AsteroidShardData {
    const float lifetime;
    float time_left;

    AsteroidShardData(float time_to_live) :
        lifetime(time_to_live),
        time_left(lifetime)
    {}
};

Uint8 shard_tick(AsteroidShardData& shard, float dt);




void generateStabber(void);

struct StabberData {
    enum class State {
        Stabbing,
        Relocating,
        Aiming
    } state = State::Relocating;

    float time_since_last_stab = 0.f;
    const float wait_time;
    const float speed;

    v2 idle_point;
    v2 stab_point;

    StabberData(void)
        : wait_time(uniform_rand(3,5))
        , speed(uniform_rand(45,55))
    {}
};

void set_stabber_to_relocating(Entity& stabber, StabberData& stabber_data);
