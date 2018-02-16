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

