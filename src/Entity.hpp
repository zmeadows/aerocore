#pragma once

#include "Base.hpp"
#include "PolygonDecomposition.hpp"
#include <SDL.h>
#include <SDL_gpu.h>

struct QuadNode;

struct DestructTag {};
struct FriendlyTag {};

struct EulerTranslation {
    v2 vel = { 0.f, 0.f };
    v2 acc = { 0.f, 0.f };
    float drag = 0.f;
};

struct LinearPath {
    v2 from, to;
    f32 t_elapsed, t_total;
};

struct EulerRotation {
    float vel = 0.f;
    float acc = 0.f;
};

//TODO: define a variant, where the position update can either
// be a delta or an absolute
struct PositionUpdate {
    v2 delta = { 0.f, 0.f };

    //TODO: add intantaneous velocity estimate?! Could be super useful...
};

struct RotationUpdate {
    float dtheta = 0.0;

    //TODO: add intantaneous velocity estimate?! Could be super useful...
};

struct CollisionData {
    const LocalVertexBuffer* const local_vertices;
    const PolygonDecomposition* const poly_decomp;
    QuadNode* node = nullptr;

    CollisionData(const LocalVertexBuffer* _local_vertices, const PolygonDecomposition* _poly_decomp)
        : local_vertices(_local_vertices), poly_decomp(_poly_decomp) {}
};

struct Entity {
    Extent extent;
    v2 pos         = { 0.f, 0.f };
    float angle    = 0.f;
};

struct Sprite {
    GPU_Image* image;
    v2 offset;
};

// void destroy_entity(const UUID& uuid);
v2 orientation_of(const Entity& entity);
bool is_offscreen(const Entity& cd);
bool overlaps( const Entity& entityA , const Entity& entityB , const CollisionData& cdA , const CollisionData& cdB);
void recompute_global_context(UUID uuid, Entity& entity);
