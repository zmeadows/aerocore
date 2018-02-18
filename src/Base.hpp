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

float uniform_rand(float min, float max);

enum class Alliance { Friend, Foe, Neutral };


struct WrapOSB { };

struct SinglePassOSB { bool found_onscreen = false; };

struct ValidRangeOSB {
    float minx = -250.f;
    float maxx = 250.f;
    float miny = -250.f;
    float maxy = 250.f;
};

typedef std::variant<SinglePassOSB, ValidRangeOSB, WrapOSB> OffscreenBehavior;

const UUID playerUUID(void);

struct CoreData {
    std::vector<v2> vertices = {};
    SDL_Color color = { 255, 255, 255, 255 };
    v2 pos = {0.f, 0.f};
    v2 vel = { 0.f, 0.f };
    v2 acc = { 0.f, 0.f };
    float angle = 0.f;
    float angvel = 0.f;
    bool filled = false;
    bool wraps = false; // TODO: remove
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

// TODO: move to Sprite file
struct Extent {
    float minX = 0.f;
    float maxX = 0.f;
    float minY = 0.f;
    float maxY = 0.f;
};

Extent clip_to_screen(const Extent& ext);

inline void rotate(float& angle, float delta) {
    angle = std::fmod(angle + delta, TWOPI);
}

