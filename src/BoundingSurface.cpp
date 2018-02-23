#include "BoundingSurface.hpp"


AxisProjection
project_on(const GlobalVertexBuffer& global_vertices,
           const PolygonRep polygon, const v2& axis)
{
    float minProjection = std::numeric_limits<float>::max();
    float maxProjection = std::numeric_limits<float>::lowest();

    //@TODO: use SIMD here, loop over entities in chunks and keep min/max of each lane in SIMD 4/8-registers
    //       then take min/max of minmax registers at the end of everything.
    for (auto idx = 0; idx < polygon.count; idx++) {
        const float projection = global_vertices[polygon.indices[idx]].dot(axis);
        minProjection = std::min(projection, minProjection);
        maxProjection = std::max(projection, maxProjection);
    }

    return AxisProjection({minProjection, maxProjection});
}

PolygonRep
nth_polygon(const PolygonDecomposition& decomp, size_t idx)
{
    assert(idx < decomp.count && "attempted to access polygon ID > N in an N-Polygon decomposition");

    const auto off2 = decomp.offsets[idx+1];
    const auto off1 = decomp.offsets[idx];

    assert( (off2 - off1 >= 3) && "mis-assigned offsets in polygon decomposition");

    return PolygonRep({ &(decomp.indices[off1]), static_cast<uint_least8_t>(off2 - off1) });
}

void
fill_polygon_normals(const GlobalVertexBuffer& global_vertices,
                     const PolygonRep polygon, v2* normals)
{
    const uint_least8_t num_vertices = polygon.count;

    for (uint_least8_t i = 0; i < num_vertices; i++) {
        v2 ov = global_vertices[polygon.indices[(i + 1) % num_vertices]]
                - global_vertices[polygon.indices[i]];
        normals[i] = { -ov.y, ov.x };
    }
}

    void
dump(const PolygonDecomposition& decomp)
{
    std::cout << std::endl;
    std::cout << "## PolygonDecomposition START ##" << std::endl;
    for (size_t pgon = 0; pgon < decomp.count; pgon++) {
        PolygonRep pr = nth_polygon(decomp, pgon);
        for (size_t i = 0; i < pr.count; i++) {
            std::cout << static_cast<size_t>(pr.indices[i]) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "## PolygonDecomposition END ##" << std::endl;
    std::cout << std::endl;
}
