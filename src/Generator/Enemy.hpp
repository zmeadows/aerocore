#pragma once

#include "Vector2D.hpp"

void generate_enemy_skeleton(UUID uuid, v2 position, const char* sprite_file, const char* path_file);

// void generateStabber(void);

// struct Stabber {
//     enum class State {
//         Stabbing,
//         Relocating,
//         Stopping,
//         Aiming
//     } state = State::Stopping;
//
//     float speed = 0.f;
//
//     v2 idle_point;
// };
