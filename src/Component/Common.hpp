#pragma once

#include "Util.hpp"
#include "Extent.hpp"
#include "Vector2D.hpp"
#include "Coordinates.hpp"

// struct Entity {
//     Extent extent;
//     v2 pos         = { 0.f, 0.f };
//     float angle    = 0.f;
// };

using Position = BoundedCoordinates<-16.f/2.f, 16.f/8.f, -9.f/2.f, 9.f/2.f>;

struct Velocity {
    f32 x = 0.f;
    f32 y = 0.f;
};

struct Acceleration {
    f32 x = 0.f;
    f32 y = 0.f;
};

Position operator*(Velocity, TimeDelta);
Position operator*(TimeDelta, Velocity);
Velocity operator*(Acceleration, TimeDelta);
Velocity operator*(TimeDelta, Acceleration);

class Orientation {
    f32 value;

public:
    f32 angle() const { return value; }

    v2 unit_vector() const {
        return { std::cos(this->value), std::sin(this->value) };
    }

    f32 rotate(f32 delta) {
        static const f32 TWO_PI = 2.f * static_cast<f32>(3.1415926535897932384);
        const f32 new_angle = fmod(this->value + delta, TWO_PI);
        this->value = (new_angle < 0) ? new_angle + TWO_PI : new_angle;
    }
};

struct EulerTranslation {
    Velocity vel = { 0.f, 0.f };
    Acceleration acc = { 0.f, 0.f };
};

struct EulerRotation {
    f32 vel = 0.f;
    f32 acc = 0.f;
};

//TODO: define a variant, where the position update can either
// be a delta or an absolute
//TODO: add intantaneous velocity estimate?! Could be useful...
struct PositionUpdate {
    v2 delta = { 0.f, 0.f };
};

struct RotationUpdate {
    float dtheta = 0.0;
};

struct LinearPath {
    v2 from, to;
    f32 t_elapsed, t_total;
};

// COMMON TAGS
struct DestructTag {};
struct FriendlyTag {};
struct BulletTag {};

struct GPU_Image;

struct Sprite {
    GPU_Image* image;
    v2 offset;
};

struct Health {
    f32 maximum = 999999.f;
    f32 current = 999999.f;

    void modify_by(f32 delta) { current = clamp(current + delta, 0.f, maximum); }
};

struct DamageEvent {
    f32 amount = 0.f;
};

struct CollideDamage {
    f32 amount = 0.5f;
};

struct Invincibility {
    f32 timer = 0.0f;
    u32 num_flashes = 8;
    bool is_flashing = false;
    u8 r = 255;
    u8 g = 0;
    u8 b = 0;
};

struct StateTransition {
    s32 next_state_id = -1;
    f32 extra_time = 0.0;
};

struct PauseBehavior {
    f32 time_left;
    s32 next_state_id;
};

