#include "BoundingSurface.hpp"

SurfaceNormalSet::SurfaceNormalSet(const std::vector<Vector2f>& vertices) {
    const size_t numVertices = vertices.size();

    normals.reserve(numVertices);

    for (size_t i = 0; i < numVertices; i++) {
        Vector2f ov = vertices[(i + 1) % numVertices] - vertices[i];
        std::swap(ov.x, ov.y);
        if (ov.x != 0.0)
            ov.x *= -1.0;
        add(ov);
    }
}

SurfaceNormalSet::SurfaceNormalSet(const SurfaceNormalSet& rhs) {
    normals.reserve(rhs.size());
    for (size_t i = 0; i < rhs.normals.size(); i++) {
        this->add(rhs.normals[i]);
    }
}

void SurfaceNormalSet::add(const Vector2f& vec) {
    static const float tol = 1e-3;

    for (const auto& ov : normals) {
        const float angle = std::atan2f(vec.y, vec.x) - std::atan2f(ov.y, ov.x);
        if (std::abs(angle) < tol || std::abs(M_PI - std::abs(angle)) < tol)
            return;
    }

    normals.push_back(vec.normalized());
}

void SurfaceNormalSet::add(const SurfaceNormalSet& rhs) {
    normals.reserve(normals.size() + rhs.size());
    for (size_t i = 0; i < rhs.normals.size(); i++) {
        this->add(rhs.normals[i]);
    }
}

void SurfaceNormalSet::add(const SurfaceNormalSet* rhs) {
    if (rhs) {
        normals.reserve(normals.size() + rhs->size());
        for (size_t i = 0; i < rhs->normals.size(); i++) {
            this->add(rhs->normals[i]);
        }
    }
}

AxisProjection
PolygonalBoundingSurface::projectOn(const Vector2f& axis, const Position& pos, const Rotation& rot) const {
    float minProjection = std::numeric_limits<float>::max();
    float maxProjection = std::numeric_limits<float>::lowest();

    for (const Vector2f& vtx : polygon.getTransRotVertices(pos, rot)) {
        const float projection = vtx.dot(axis);
        minProjection = std::min(projection, minProjection);
        maxProjection = std::max(projection, maxProjection);
    }

    return AxisProjection({minProjection, maxProjection});
}

bool overlaps(const BoundingSurface& bsA,
              const Position& posA,
              const Rotation& rotA,
              const BoundingSurface& bsB,
              const Position& posB,
              const Rotation& rotB) {
    // combine surface normals into one SurfaceNormalSet so that
    // duplicates are removed, rather than separately looping over both sets
    SurfaceNormalSet combinedSurfaceNormals;
    combinedSurfaceNormals.add(bsA.getSurfaceNormals());
    combinedSurfaceNormals.add(bsB.getSurfaceNormals());

    for (const Vector2f& axis : combinedSurfaceNormals) {
        AxisProjection projA = bsA.projectOn(axis, posA, rotA);
        AxisProjection projB = bsB.projectOn(axis, posB, rotB);
        if (!(projA.max >= projB.min && projB.max >= projA.min)) {
            return false;
        }
    }

    return true;
}