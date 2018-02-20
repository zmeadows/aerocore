#include "Geometry.hpp"

int modulo(int num, int mod) {
    if (num < 0) {
        return (num + (1 + -num/mod)*mod) % mod;
    } else {
        return num % mod;
    }
}


v2 average_vec(const std::vector<v2>& vertices) {
    v2 avg;

    for (const v2& vec : vertices)
        avg = avg + vec;

    avg.scale(1.f / vertices.size());
    return avg;
}

bool is_convex(const std::vector<v2>& vertices) {
    const size_t nvert = vertices.size();

    for (int i = 0; i < static_cast<int>(vertices.size()); i++) {
        if (cross(vertices[modulo(i-1, nvert)] - vertices[static_cast<size_t>(i)],
                  vertices[modulo(i+1, nvert)] - vertices[static_cast<size_t>(i)]) <= 0)
            return false;
    }

    return true;
};

// decompose a polygon into triangles if not convex.
std::vector<std::vector<size_t>> decompose_into_triangle_indices(const std::vector<v2>& vertices)
{
    std::vector<std::vector<size_t>> final_indices;

    const size_t vertex_count = vertices.size();

    std::vector<size_t> remaining_indices;
    remaining_indices.reserve(vertex_count);
    for (size_t i = 0; i < vertex_count; i++) {
        remaining_indices.push_back(i);
    }

    if (is_convex(vertices)) {
        final_indices.push_back(remaining_indices);
        return final_indices;
    }

    const size_t num_expected_triangles = vertex_count - 2;

    // for now we only do triangulation.
    // final_indices.resize(num_expected_triangles);
    // for (std::vector<size_t>& triangle : final_indices)
    //     triangle.resize(3);

    while (remaining_indices.size() > 2)
    {
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

            const std::vector<v2> candidate_triangle = {
                vertices[candidate_indices[0]],
                vertices[candidate_indices[1]],
                vertices[candidate_indices[2]]
            };

            if (cross(candidate_triangle[0] - candidate_triangle[1],
                      candidate_triangle[2] - candidate_triangle[1]) <= 0) {
                continue;
            }

            bool candidate_passes = true;

            for (size_t any_idx = 0; any_idx < vertices.size(); any_idx++) {
                if (!vector_contains(candidate_indices, any_idx)
                    && pnpoly(candidate_triangle, vertices[any_idx]))
                {
                    candidate_passes = false;
                    break;
                }
            }

            if (candidate_passes) {
                final_indices.push_back(candidate_indices);
                num_triangles_found++;
                remaining_indices.erase(tmp_it);
                if (num_triangles_found == vertices.size() - 2)
                    break;
            }
        }

        if (num_triangles_found == 0) break;
    }

    return final_indices;
}

bool pnpoly(const std::vector<v2>& vertices, const v2& test)
{
    const size_t nvert = vertices.size();

    int c = 0;
    size_t i, j;

    for (i = 0, j = nvert-1; i < nvert; j = i++) {
        if ( ((vertices[i].y>test.y) != (vertices[j].y>test.y)) &&
            (test.x < (vertices[j].x-vertices[i].x) * (test.y-vertices[i].y) / (vertices[j].y-vertices[i].y) + vertices[i].x) )
            c = !c;
    }

    return static_cast<bool>(c);
}
