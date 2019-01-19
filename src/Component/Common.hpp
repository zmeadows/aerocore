#pragma once

#include "Extent.hpp"
#include "Vector2D.hpp"

struct GPU_Image;

struct Entity {
    Extent extent;
    v2 pos         = { 0.f, 0.f };
    float angle    = 0.f;
};

v2 orientation_of(const Entity& entity);
bool is_offscreen(const Entity& entity);

struct EulerTranslation {
    v2 vel = { 0.f, 0.f };
    v2 acc = { 0.f, 0.f };
    float drag = 0.f;
};

struct EulerRotation {
    float vel = 0.f;
    float acc = 0.f;
};

//TODO: define a variant, where the position update can either
// be a delta or an absolute
//TODO: add intantaneous velocity estimate?! Could be super useful...
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

struct Sprite {
    GPU_Image* image;
    v2 offset;
};

