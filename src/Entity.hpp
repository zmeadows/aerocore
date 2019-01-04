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

struct PositionUpdate {
    v2 delta = { 0.f, 0.f };
};

struct RotationUpdate {
    float dtheta = 0.0;
};

struct CollisionData {
    LocalVertexBuffer local_vertices;
    GlobalVertexBuffer global_vertices;
    //@TODO: don't *need* to make copies of polygon decompositions for each copy of a sprite
    // maybe it is still best, since they are small?
    PolygonDecomposition poly_decomp;
    QuadNode* node = nullptr;
};

struct Sprite {
    GPU_Image* image;
    v2 offset;
};

struct Entity {
    Extent extent;
    v2 pos         = { 0.f, 0.f };
    float angle    = 0.f;
};

// void destroy_entity(const UUID& uuid);
v2 orientation_of(const Entity& entity);
bool is_offscreen(const Entity& cd);
bool overlaps(const CollisionData& entityA, const CollisionData& entityB,
              const Extent extA, const Extent extB);
void recompute_global_context(UUID uuid, Entity& entity);
