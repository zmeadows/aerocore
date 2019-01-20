#include "PolygonDecomposition.hpp"

#include <algorithm>
#include <limits>
#include <vector>

#include "LocalVertexBuffer.hpp"
#include "Util.hpp"
#include "Vector2D.hpp"
#include "Geometry.hpp"

// test if a point lies within an arbitrary polygon
bool pnpoly(const v2* vertices, const u32 nvert, const v2& test) {
    int c = 0;
    u32 i, j;

    for (i = 0, j = nvert - 1; i < nvert; j = i++) {
        if (((vertices[i].y > test.y) != (vertices[j].y > test.y)) &&
            (test.x <
             (vertices[j].x - vertices[i].x) * (test.y - vertices[i].y) / (vertices[j].y - vertices[i].y) +
                 vertices[i].x))
            c = !c;
    }

    return static_cast<bool>(c);
}

AxisProjection project_on(const std::vector<v2>& global_vertices, const PolygonRep polygon, const v2& axis) {
    float minProjection = std::numeric_limits<float>::max();
    float maxProjection = std::numeric_limits<float>::lowest();

    for (auto idx = 0; idx < polygon.count; idx++) {
        const auto gidx = polygon.indices[idx];
        assert(gidx < global_vertices.size());
        const f32 projection = global_vertices[gidx].dot(axis);
        minProjection = std::min(projection, minProjection);
        maxProjection = std::max(projection, maxProjection);
    }

    return AxisProjection({minProjection, maxProjection});
}

PolygonRep nth_polygon(const PolygonDecomposition* decomp, u32 idx) {
    assert(idx < decomp->count && "attempted to access polygon ID > N in an N-Polygon decomposition");

    const auto off2 = decomp->offsets[idx + 1];
    const auto off1 = decomp->offsets[idx];

    assert((off2 - off1 >= 3) && "mis-assigned offsets in polygon decomposition");

    return PolygonRep({&(decomp->indices[off1]), static_cast<uint_least8_t>(off2 - off1)});
}

void fill_polygon_normals(const std::vector<v2>& global_vertices, const PolygonRep polygon, v2* normals) {
    const uint_least8_t num_vertices = polygon.count;

    for (uint_least8_t i = 0; i < num_vertices; i++) {
        const auto idx1 = polygon.indices[(i + 1) % num_vertices];
        const auto idx2 = polygon.indices[i];

        assert(idx1 < global_vertices.size());
        assert(idx2 < global_vertices.size());

        v2 ov = global_vertices[idx1] - global_vertices[idx2];
        normals[i] = {-ov.y, ov.x};
    }
}

void dump(const PolygonDecomposition* decomp) {
    printf("\n## PolygonDecomposition START ##\n");

    for (u32 pgon = 0; pgon < decomp->count; pgon++) {
        PolygonRep pr = nth_polygon(decomp, pgon);
        for (u32 i = 0; i < pr.count; i++) {
            printf((i == pr.count - 1) ? "%u\n" : "%u ", static_cast<u32>(pr.indices[i]));
        }
    }

    printf("\n## PolygonDecomposition END ##\n");
}

PolygonDecomposition decompose_local_vertices(const LocalVertexBuffer* local_vertices) {
    const u32 vertex_count = local_vertices->count;

    std::vector<std::vector<u32>> final_indices;

    std::vector<u32> remaining_indices;
    remaining_indices.reserve(vertex_count);
    for (u32 i = 0; i < vertex_count; i++) {
        remaining_indices.push_back(i);
    }

    if (is_convex(*local_vertices)) {
        final_indices.push_back(remaining_indices);
        remaining_indices.clear();
    }

    while (remaining_indices.size() >= 3) {
        if (is_convex(*local_vertices)) {
            final_indices.push_back(remaining_indices);
            remaining_indices.clear();
        }

        //@TODO: check if convex after each iteration!

        // copy so that the indices we are looping over
        // are not the same set we are removing used indices from
        const std::vector<u32> loop_indices(remaining_indices);

        u32 num_triangles_found = 0;
        for (const u32 i : loop_indices) {

            auto tmp_it = std::find(remaining_indices.begin(), remaining_indices.end(), i);
            assert(tmp_it != remaining_indices.end());

            // index of the index...
            const int ii = static_cast<int>(std::distance(remaining_indices.begin(), tmp_it));
            assert(ii >= 0);

            const u32 num_remaining_indices = remaining_indices.size();

            const std::vector<u32> candidate_indices = {
                remaining_indices[modulo(ii - 1, static_cast<int>(num_remaining_indices))],
                remaining_indices[static_cast<u32>(ii)],
                remaining_indices[modulo(ii + 1, static_cast<int>(num_remaining_indices))]};

            const v2 candidate_triangle[3] = {(*local_vertices)[candidate_indices[0]],
                                              (*local_vertices)[candidate_indices[1]],
                                              (*local_vertices)[candidate_indices[2]]};

            if (cross(candidate_triangle[0] - candidate_triangle[1],
                      candidate_triangle[2] - candidate_triangle[1]) <= 0) {
                continue;
            }

            bool candidate_passes = true;

            for (u32 any_idx = 0; any_idx < vertex_count; any_idx++) {
                if (!vector_contains(candidate_indices, any_idx) &&
                    pnpoly(candidate_triangle, 3, (*local_vertices)[any_idx])) {
                    candidate_passes = false;
                    break;
                }
            }

            if (candidate_passes) {
                final_indices.push_back(candidate_indices);
                num_triangles_found++;
                remaining_indices.erase(tmp_it);
                if (num_triangles_found == vertex_count - 2)
                    break;
            }
        }

        if (num_triangles_found == 0)
            break;

        // {
		// 	//OPTIMIZE: when/where this will find a convex sub-polygon depends on the starting point used
		// 	//	        perhaps there is a smart way to optimize this to find the simplest polygon decomposition.
        //     std::vector<v2> convex_test_vertices;
        //     convex_test_vertices.reserve(remaining_indices.size());

        //     for (auto idx : remaining_indices) {
        //         convex_test_vertices.push_back((*local_vertices)[idx]);
        //     }

        //     if (is_convex(convex_test_vertices)) {
        //         final_indices.push_back(remaining_indices);
        //         remaining_indices.clear();
        //     }
        // }
    }

    PolygonDecomposition poly_decomp;

    u32 idx = 0;
    poly_decomp.offsets[0] = 0;
    poly_decomp.count = 0;
    for (const std::vector<u32>& tridxs : final_indices) {
        assert(tridxs.size() <= 255);
        poly_decomp.count++;
        poly_decomp.offsets[poly_decomp.count] =
            poly_decomp.offsets[poly_decomp.count - 1] + static_cast<uint_least8_t>(tridxs.size());
        for (u32 tridx : tridxs) {
            assert(tridx <= 255);
            poly_decomp.indices[idx] = static_cast<uint_least8_t>(tridx);
            idx++;
        }
    }

    return poly_decomp;
}
