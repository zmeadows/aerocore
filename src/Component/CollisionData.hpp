#pragma once

struct LocalVertexBuffer;
struct PolygonDecomposition;
struct QuadNode;

struct CollisionData {
    const LocalVertexBuffer* const local_vertices;
    const PolygonDecomposition* const poly_decomp;
    QuadNode* node = nullptr;

    CollisionData(const LocalVertexBuffer* _local_vertices, const PolygonDecomposition* _poly_decomp)
        : local_vertices(_local_vertices), poly_decomp(_poly_decomp) {}
};
