#include "Geometry.hpp"
#include "BoundingSurface.hpp"

void
recenter(LocalVertexBuffer& local_vertices)
{
    const v2 offset = average_vector(local_vertices.data, local_vertices.count);

    for (size_t i = 0; i < local_vertices.count; i++)
        local_vertices[i] = local_vertices[i] - offset;
};

int modulo(int num, int mod) {
    if (num < 0) {
        return (num + (1 + -num/mod)*mod) % mod;
    } else {
        return num % mod;
    }
}

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
    const size_t vertex_count = vertices.count;

    //@FIXME: this is gross
    for (int i = 0; i < static_cast<int>(vertex_count); i++) {
        if (cross(vertices[modulo(i-1, static_cast<int>(vertex_count))] - vertices[static_cast<size_t>(i)],
                  vertices[modulo(i+1, static_cast<int>(vertex_count))] - vertices[static_cast<size_t>(i)]) <= 0)
            return false;
    }

    return true;
};

// decompose a polygon into triangles if not convex.
PolygonDecomposition
decompose_local_vertices(const LocalVertexBuffer& local_vertices)
{
    const size_t vertex_count = local_vertices.count;

    std::vector<std::vector<size_t>> final_indices;

    std::vector<size_t> remaining_indices;
    remaining_indices.reserve(vertex_count);
    for (size_t i = 0; i < vertex_count; i++) {
        remaining_indices.push_back(i);
    }

    // if (is_convex(local_vertices, vertex_count)) {
    //     final_indices.push_back(remaining_indices);
    //     return final_indices;
    // }

    const size_t num_expected_triangles = vertex_count - 2;

    while (remaining_indices.size() >= 3)
    {
        //@TODO: check if convex after each iteration!

        // copy so that the indices we are looping over
        // are not the same set we are removing used indices from
        const std::vector<size_t> loop_indices(remaining_indices);

        size_t num_triangles_found = 0;
        for (const size_t i : loop_indices) {

            auto tmp_it = std::find(remaining_indices.begin(), remaining_indices.end(), i);
            assert(tmp_it != remaining_indices.end());

            // index of the index...
            const int ii = static_cast<int>(std::distance(remaining_indices.begin(), tmp_it));
            assert(ii >= 0);

            const size_t num_remaining_indices = remaining_indices.size();

            const std::vector<size_t> candidate_indices = {
                remaining_indices[modulo(ii - 1, num_remaining_indices)],
                remaining_indices[static_cast<size_t>(ii)],
                remaining_indices[modulo(ii + 1 , num_remaining_indices)]
            };

            // std::cout << "candidates: "
            //     << candidate_indices[0] << " "
            //     << candidate_indices[1] << " "
            //     << candidate_indices[2] << std::endl;

            const v2 candidate_triangle[3] = {
                local_vertices[candidate_indices[0]],
                local_vertices[candidate_indices[1]],
                local_vertices[candidate_indices[2]]
            };

            if (cross(candidate_triangle[0] - candidate_triangle[1],
                      candidate_triangle[2] - candidate_triangle[1]) <= 0) {
                continue;
            }

            bool candidate_passes = true;

            for (size_t any_idx = 0; any_idx < vertex_count; any_idx++) {
                //@ALERT: might need
                if (!vector_contains(candidate_indices, any_idx) && pnpoly(candidate_triangle, 3, local_vertices[any_idx])) {
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

        if (num_triangles_found == 0) break;
    }

    PolygonDecomposition poly_decomp;

    //@FIXME: doing nothing atm
    size_t idx = 0;
    poly_decomp.offsets[0] = 0;
    poly_decomp.count = 0;
    for (const std::vector<size_t>& tridxs : final_indices) {
        poly_decomp.count++;
        poly_decomp.offsets[poly_decomp.count] = poly_decomp.offsets[poly_decomp.count-1] + tridxs.size();
        for (size_t tridx : tridxs) {
            poly_decomp.indices[idx] = tridx;
            idx++;
        }
    }

    // dump(poly_decomp);


    return poly_decomp;
}

bool pnpoly(const v2* vertices, const size_t nvert, const v2& test)
{
    int c = 0;
    size_t i, j;

    for (i = 0, j = nvert-1; i < nvert; j = i++) {
        if ( ((vertices[i].y>test.y) != (vertices[j].y>test.y)) &&
            (test.x < (vertices[j].x-vertices[i].x) * (test.y-vertices[i].y) / (vertices[j].y-vertices[i].y) + vertices[i].x) )
            c = !c;
    }

    return static_cast<bool>(c);
}
