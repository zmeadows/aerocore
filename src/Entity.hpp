#pragma once

#include "Base.hpp"
#include "PolygonDecomposition.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_gpu.h>

struct QuadNode;

//@FIXME: play with size/packing of Entity
struct Entity {
    LocalVertexBuffer local_vertices;
    GlobalVertexBuffer global_vertices;
    PolygonDecomposition poly_decomp;
    GPU_Image* sprite;
    v2 sprite_offset;



    Extent extent;
    OffscreenBehavior osb;

    //@TODO  make this a single 32bit number?
    SDL_Color color                 = { 255, 255, 255, 255 };

    QuadNode* node = nullptr;

    v2 pos                          = { 0.f, 0.f };
    v2 vel                          = { 0.f, 0.f };
    v2 acc                          = { 0.f, 0.f };
    float angle                     = 0.f;
    float angvel                    = 0.f;
    float drag                      = 0.f;
    bool offscreen                  = true;
    bool friendly                   = true;
};

Extent extent_of(const Entity& cd);
void destroy_entity(const UUID& uuid);
v2 orientation_of(const Entity& entity);
bool is_offscreen(const Entity& cd);
bool overlaps(const Entity& entityA, const Entity& entityB);
void recompute_global_context(UUID uuid, Entity& entity);
