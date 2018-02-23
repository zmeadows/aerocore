#pragma once

#include "Entity.hpp"

UUID generateOffscreenAsteroid(void);

void generateAsteroidShards(const Entity& asteroid,
                            const std::vector<std::vector<size_t>>& triangles_indices);

struct AsteroidShardData {
    float lifetime = 0.0;
    float time_left = 0.0;
};

Uint8 shard_tick(AsteroidShardData& shard, float dt);

void generateStabber(void);

struct StabberData {
    enum class State {
        Stabbing,
        Relocating,
        Stopping,
        Aiming
    } state = State::Relocating;

    float time_since_last_stab = 0.f;
    float wait_time = 0.f;
    float speed = 0.f;

    v2 idle_point;
    v2 stab_point;
};

void set_stabber_to_relocating(Entity& stabber, StabberData& stabber_data);
