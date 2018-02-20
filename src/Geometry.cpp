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
    const int nvert = vertices.size();

    for (size_t i = 0; i < vertices.size(); i++) {
        if (cross(vertices[modulo(i-1, nvert)] - vertices[i],
                  vertices[modulo(i+1, nvert)] - vertices[i]) <= 0)
            return false;
    }

    return true;
};

std::vector<std::vector<v2>> decompose(const std::vector<v2>& vertices)
{
    if (is_convex(vertices)) return { vertices };

    std::vector<std::vector<v2>> triangles;

    std::vector<v2> vtx_list(vertices);
    while (triangles.size() < vertices.size() - 2) {
        int num_triangles_found = 0;
        const std::vector<v2> loop_vertices(vtx_list);
        for (const v2& vtx : loop_vertices) {
            auto minx = std::find(vtx_list.begin(), vtx_list.end(), vtx);

            assert(minx != vtx_list.end());

            const int idx = static_cast<int>(std::distance(vtx_list.begin(), minx));
            const int nvert = vtx_list.size();

            const std::vector<int> candidate_indices = { modulo(idx - 1, nvert), idx, modulo(idx + 1 , nvert) };
            const std::vector<v2> candidate_triangle = {
                vtx_list[candidate_indices[0]],
                vtx_list[candidate_indices[1]],
                vtx_list[candidate_indices[2]]
            };

            if (cross(candidate_triangle[0] - candidate_triangle[1],
                      candidate_triangle[2] - candidate_triangle[1]) <= 0) {
                continue;
            }

            bool candidate_passes = true;

            for (int i = 0; i < vertices.size(); i++) {
                if (!vector_contains(candidate_triangle, vertices[i]) && pnpoly(candidate_triangle, vertices[i])) {
                    candidate_passes = false;
                    break;
                }
            }

            if (candidate_passes) {
                triangles.push_back(candidate_triangle);
                num_triangles_found++;
                vtx_list.erase(minx);
                if (num_triangles_found == vertices.size() - 2)
                    break;
            }
        }

        if (num_triangles_found == 0) break;
    }

    // assert(triangles.size() == vertices.size() - 2);

    return triangles;
}

bool pnpoly(const std::vector<v2>& vertices, const v2& test)
{
    const int nvert = vertices.size();

    int i, j, c = 0;

    for (i = 0, j = nvert-1; i < nvert; j = i++) {
        if ( ((vertices[i].y>test.y) != (vertices[j].y>test.y)) &&
            (test.x < (vertices[j].x-vertices[i].x) * (test.y-vertices[i].y) / (vertices[j].y-vertices[i].y) + vertices[i].x) )
            c = !c;
    }

    return static_cast<bool>(c);
}
