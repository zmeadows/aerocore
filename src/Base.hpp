#pragma once

#include "Vector2D.hpp"

#include <SDL2/SDL.h>

#include <cmath>
#include <functional>
#include <iostream>

#ifdef DODEBUG
#define DEBUG(x)                                                                                             \
    (std::cout << std::boolalpha << "[" << __FILE__ << "::" << __LINE__ << "]\t" << x << std::endl)
#else
#define DEBUG(x)
#endif

enum class EntityType { Player, Bullet, Enemy, Particle, Level };

enum class Alliance { Friend, Foe, Neutral };

struct RGBA {
    uint_fast8_t r = 0;
    uint_fast8_t g = 0;
    uint_fast8_t b = 0;
    uint_fast8_t a = 0;
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
};

struct DeathTimer {
    float value = 0.0;
};

struct ShotDelay {
    const float delay = 0.0;
    float lastShotTime = 0.0;
};

struct Extent {
    float minX = 0.f;
    float maxX = 0.f;
    float minY = 0.f;
    float maxY = 0.f;
};

struct Position {
    float x = 0.f;
    float y = 0.f;

    inline Vector2f toVec(void) { return {x, y}; }
};

struct ScreenCoordinates {
    Sint16 x = 0;
    Sint16 y = 0;
};

struct Velocity {
    float x = 0.f;
    float y = 0.f;

    inline Vector2f toVec(void) { return {x, y}; }
};

struct Acceleration {
    float x = 0.f;
    float y = 0.f;

    inline Vector2f toVec(void) { return {x, y}; }
};

struct RotationalVelocity {
    float value = 0.f;
};

class Rotation {
private:
    float angle = 0.f;

public:
    inline float getAngle(void) const { return angle; }

    inline void rotateAngle(float radians) {
        if (radians >= 0) {
            angle = std::fmod(angle + radians, 2 * M_PI);
        } else {
            angle = std::fmod(angle + radians + 2 * M_PI, 2 * M_PI);
        }
    }
};

enum class OffscreenBehavior { DiesInstantly, Wraps };
