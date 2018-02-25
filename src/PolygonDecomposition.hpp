#pragma once

#include <cstdint>
#include "Typedef.hpp"

struct PolygonDecomposition;
struct GlobalVertexBuffer;
struct LocalVertexBuffer;
struct v2;
struct PolygonRep;

struct AxisProjection {
    float min = 0.f;
    float max = 0.f;
};


//@GAMEIDEA: very limited ammo! So that the player has to choose between shooting asteroids (frees up space)
//and shooting enemies (progresses through level or gets rewards/points)
//no points for killing asteroids!
//... asteroids roguelike! :)
//

//@GAMEIDEA: paint with your ship  somehow? screen full of asteroids, invinclbe, piercing bullets?


struct PolygonDecomposition {
    uint_least8_t indices[256];
    uint_least8_t offsets[127];
    uint_least8_t count = 0;
};

struct PolygonRep {
    const uint_least8_t* indices = nullptr;
    const uint_least8_t count = 0;
};


PolygonRep nth_polygon(const PolygonDecomposition& decomp, u32 idx);
void fill_polygon_normals(const GlobalVertexBuffer& vertices, const PolygonRep polygon, v2* normals);
AxisProjection project_on(const GlobalVertexBuffer& vertices, const PolygonRep polygon, const v2& axis);
void dump(const PolygonDecomposition& decomp);

PolygonDecomposition decompose_local_vertices(const LocalVertexBuffer& local_vertices);

bool pnpoly(const v2* vertices, const u32 nvert, const v2& test);
