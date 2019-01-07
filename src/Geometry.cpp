#include "Geometry.hpp"

#include "Vector2D.hpp"
#include "Base.hpp"
#include "Util.hpp"

#include <iostream>

v2
recenter(LocalVertexBuffer& local_vertices)
{
    const v2 offset = average_vector(local_vertices.data, local_vertices.count);

    for (size_t i = 0; i < local_vertices.count; i++)
        local_vertices[i] = local_vertices[i] - offset;

    return offset;
};


v2 average_vector(const v2* vertices, const size_t vertex_count) {
    v2 average_vertex = { 0.f, 0.f };

    for (size_t i = 0; i < vertex_count; i++)
        average_vertex = average_vertex + vertices[i];

    average_vertex.scale(1.f / static_cast<float>(vertex_count));

    return average_vertex;
}

bool
is_convex(const LocalVertexBuffer& vertices)
{
    const s32 vertex_count = static_cast<s32>(vertices.count);
    assert(vertex_count >= 0);

    for (s32 i = 0; i < vertex_count; i++) {
        if (cross(vertices[modulo(i-1, vertex_count)] - vertices[unsign(i)],
                  vertices[modulo(i+1, vertex_count)] - vertices[unsign(i)]) <= 0)
            return false;
    }

    return true;
};

v2 average_vector(const LocalVertexBuffer& global_vertices) {
    return average_vector(global_vertices.data, global_vertices.count);
}
