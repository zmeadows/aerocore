#pragma once

#include <cmath>
#include <functional>
#include <iostream>
#include <stdlib.h>
#include <variant>

#include <SDL2/SDL.h>

#include "aerocore.hpp"
#include "UUID.hpp"
#include "Vector2D.hpp"

typedef float f32;
typedef double f64;

float uniform_rand(float min, float max);

enum class EntityType {
    Player,
    Asteroid,
    Bullet,
    Effect,
    Stabber
};

enum class AIType {
};

struct DiesInstantlyOSB { };

struct WrapOSB { };

// FIXME: rename better: already_found_onscreen_once
struct SinglePassOSB { bool found_onscreen = false; };

struct ValidRangeOSB {
    float minx = -250.f;
    float maxx = 250.f;
    float miny = -250.f;
    float maxy = 250.f;
};

typedef std::variant<SinglePassOSB, ValidRangeOSB, WrapOSB, DiesInstantlyOSB> OffscreenBehavior;

const UUID playerUUID(void);

//@TODO: move to Sprite file
struct Extent {
    float minX = 0.f;
    float maxX = 0.f;
    float minY = 0.f;
    float maxY = 0.f;
};

//@FIXME: play with size/packing of Entity
struct Entity {
    std::vector<v2> local_vertices  = {};
    std::vector<v2> global_vertices = {};
    OffscreenBehavior osb           = DiesInstantlyOSB();
    Extent extent                   = Extent();
    SDL_Color color                 = { 255, 255, 255, 255 };
    v2 pos                          = {0.f, 0.f};
    v2 vel                          = { 0.f, 0.f };
    v2 acc                          = { 0.f, 0.f };
    EntityType type                 = EntityType::Effect;
    float angle                     = 0.f;
    float angvel                    = 0.f;
    bool offscreen                  = true;
};


inline float signum(float num) {
    if (num > 0) {
        return 1;
    } else if (num < 0) {
        return -1;
    } else {
        return 0;
    }
}

struct ParticleGenerator {
    std::function<void(void)> generate;
    ParticleGenerator(std::function<void(void)> _generate)
        : generate(_generate) {}
};

struct DeathTimer {
    float value = 0.0;
};

struct ShotDelay {
    float delay = 0.0;
    float lastShotTime = 0.0;
};


Extent clip_to_screen(const Extent& ext);

inline float rotate(float& old_angle, float delta) {
    const float new_angle = fmod(old_angle + delta, TWOPI);
    return (new_angle < 0) ? new_angle + TWOPI : new_angle;
}

inline float arctan(float x, float y) {
    float angle = atan2(y,x);
    return (angle < 0) ? angle + TWOPI : angle;
}

