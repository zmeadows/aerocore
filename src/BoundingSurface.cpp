#include "BoundingSurface.hpp"

SurfaceNormalSet::SurfaceNormalSet(const std::vector<v2>& vertices) {
    const size_t numVertices = vertices.size();

    normals.reserve(numVertices);

    for (size_t i = 0; i < numVertices; i++) {
        v2 ov = vertices[(i + 1) % numVertices] - vertices[i];
        std::swap(ov.x, ov.y);
        if (ov.x != 0.f)
            ov.x *= -1.0;
        add(ov);
    }
}

SurfaceNormalSet::SurfaceNormalSet(const std::vector<std::vector<v2>>& vertices) {
    for (size_t j = 0; j < vertices.size(); j++) {
        const size_t numVertices = vertices[j].size();
        for (size_t i = 0; i < numVertices; i++) {
            v2 ov = vertices[j][(i + 1) % numVertices] - vertices[j][i];
            std::swap(ov.x, ov.y);
            if (ov.x != 0.f)
                ov.x *= -1.0;
            add(ov);
        }
    }
}

SurfaceNormalSet::SurfaceNormalSet(const SurfaceNormalSet& rhs) {
    normals.reserve(rhs.size());
    for (size_t i = 0; i < rhs.normals.size(); i++) {
        this->add(rhs.normals[i]);
    }
}

void SurfaceNormalSet::add(const v2& vec, float rotationAngle) {
    static const float tol = 0.001f;

    for (const auto& ov : normals) {
        const v2 rov = rotationAngle != 0.f ? ov.rotated(rotationAngle) : ov;
        const float angle = std::atan2f(vec.y, vec.x) - std::atan2f(rov.y, rov.x);
        if (std::abs(angle) < tol || std::abs(PI - std::abs(angle)) < tol)
            return;
    }

    normals.push_back(vec.normalized().rotated(rotationAngle));
}

void SurfaceNormalSet::add(const SurfaceNormalSet& rhs, float rotationAngle) {
    normals.reserve(normals.size() + rhs.size());
    for (size_t i = 0; i < rhs.normals.size(); i++) {
        this->add(rhs.normals[i], rotationAngle);
    }
}

void SurfaceNormalSet::add(const SurfaceNormalSet* rhs, float rotationAngle) {
    if (rhs) {
        normals.reserve(normals.size() + rhs->size());
        for (size_t i = 0; i < rhs->normals.size(); i++) {
            this->add(rhs->normals[i], rotationAngle);
        }
    }
}

bool overlaps(const CollisionData& colA,
              const v2& posA,
              const float rotA,
              const CollisionData& colB,
              const v2& posB,
              const float rotB) {

    if (colA.node && colB.node && colA.node != colB.node) {
        const int delta_depth = static_cast<int>(colA.node->c_depth) - static_cast<int>(colB.node->c_depth);

        if (delta_depth == 0) {
            return false;
        } else if (delta_depth > 0 && !colA.node->has_parent(colB.node)) {
            return false;
        } else if (delta_depth < 0 && !colB.node->has_parent(colA.node)) {
            return false;
        }
    }
    // TODO: check simple bounding box overlap first
    // combine surface normals into one SurfaceNormalSet so that
    // duplicates are removed, rather than separately looping over both sets
    SurfaceNormalSet combinedSurfaceNormals;
    combinedSurfaceNormals.add(colA.normals, rotA);
    combinedSurfaceNormals.add(colB.normals, rotB);

    for (const std::vector<v2>& trA : colA.triangles) {
        for (const std::vector<v2>& trB : colB.triangles) {
            bool collided = true;
            for (const v2& axis : combinedSurfaceNormals) {
                AxisProjection projA = project_on(trA, axis, posA, rotA);
                AxisProjection projB = project_on(trB, axis, posB, rotB);
                if (!(projA.max >= projB.min && projB.max >= projA.min)) {
                    collided = false;
                    break;
                }
            }
            if (collided) { return true; }
        }
    }

    return false;
}

AxisProjection
project_on(const std::vector<v2>& vertices, const v2& axis, const v2& pos, const float rot) {
    float minProjection = std::numeric_limits<float>::max();
    float maxProjection = std::numeric_limits<float>::lowest();

    for (const v2& vtx : transform_vertices(vertices, pos, rot)) {
        const float projection = vtx.dot(axis);
        minProjection = std::min(projection, minProjection);
        maxProjection = std::max(projection, maxProjection);
    }

    return AxisProjection({minProjection, maxProjection});
}
