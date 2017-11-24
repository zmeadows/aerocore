#pragma once

#include <cassert>
#include <limits>
#include <math.h>
#include <vector>

#include <Eigen/Dense>

#include "Base.hpp"

using namespace Eigen;

class SurfaceNormalSet {
private:
    std::vector<Vector2f> normals;

public:
    inline size_t size(void) const { return normals.size(); }

    void add(const Vector2f& vec) {
        static const float tol = 1e-3;

        for (const auto& ov : normals) {
            const float angle =
                std::atan2(vec.y(), vec.x()) - std::atan2f(ov.y(), ov.x());
            if (std::abs(angle) < tol || std::abs(M_PI - std::abs(angle)) < tol)
                return;
        }

        normals.push_back(vec);
    }

    void add(const SurfaceNormalSet& rhs) {
        for (size_t i = 0; i < rhs.normals.size(); i++) {
            this->add(rhs.normals[i]);
        }
    }

    std::vector<Vector2f>::const_iterator begin(void) const {
        return normals.begin();
    }

    std::vector<Vector2f>::const_iterator end(void) const {
        return normals.end();
    }

    SurfaceNormalSet(const std::vector<Vector2f>& vertices) {
        const size_t numVertices = vertices.size();

        for (size_t i = 0; i < numVertices; i++) {
            Vector2f ov = vertices[(i + 1) % numVertices] - vertices[i];
            std::swap(ov.x(), ov.y());
            if (ov.x() != 0.0)
                ov.x() *= -1.0;
            add(ov.normalized());
        }
    }

    SurfaceNormalSet(const SurfaceNormalSet& rhs) {
        for (size_t i = 0; i < rhs.normals.size(); i++) {
            this->add(rhs.normals[i]);
        }
    }

    SurfaceNormalSet(void) {}
};

struct BoundingSurface {
    std::vector<Vector2f> vertices;
    SurfaceNormalSet normals;

    BoundingSurface(const std::vector<Vector2f>& vertices_)
        : vertices(vertices_), normals(vertices) {}

    virtual ~BoundingSurface(void) {}
};

bool isSeparatingAxis(const Vector2f& axisCandidate,
                      const std::vector<Vector2f>& verticesA,
                      const Vector2f& positionA,
                      const std::vector<Vector2f>& verticesB,
                      const Vector2f& positionB) {

    float minProjectionA = std::numeric_limits<float>::max();
    float minProjectionB = std::numeric_limits<float>::max();
    float maxProjectionA = std::numeric_limits<float>::lowest();
    float maxProjectionB = std::numeric_limits<float>::lowest();

    for (const Vector2f& vtx : verticesA) {
        const float projection = (vtx + positionA).dot(axisCandidate);
        minProjectionA = std::min(projection, minProjectionA);
        maxProjectionA = std::max(projection, maxProjectionA);
    }

    for (const Vector2f& vtx : verticesB) {
        const float projection = (vtx + positionB).dot(axisCandidate);
        minProjectionB = std::min(projection, minProjectionB);
        maxProjectionB = std::max(projection, maxProjectionB);
    }

    return !(maxProjectionA >= minProjectionB &&
             maxProjectionB >= minProjectionA);
}

struct BoundingBox final : BoundingSurface {
    BoundingBox(const float halfWidth)
        : BoundingSurface({{-halfWidth, -halfWidth},
                           {halfWidth, -halfWidth},
                           {halfWidth, halfWidth},
                           {-halfWidth, halfWidth}}) {}
};

bool overlaps(const BoundingSurface* bsA, const Position* posA,
              const BoundingSurface* bsB, const Position* posB) {

    const Vector2f positionVectorA(posA->x, posA->y);
    const Vector2f positionVectorB(posB->x, posB->y);

    DEBUG("overlaps: positionA: " << positionVectorA.x() << " "
                                  << positionVectorA.y());
    DEBUG("overlaps: positionB: " << positionVectorB.x() << " "
                                  << positionVectorB.y());

    SurfaceNormalSet combinedSurfaceNormals;
    combinedSurfaceNormals.add(bsA->normals);
    combinedSurfaceNormals.add(bsB->normals);

    DEBUG("overlaps: combined normals size: " << combinedSurfaceNormals.size());

    for (const auto& axis : combinedSurfaceNormals) {
        if (isSeparatingAxis(axis, bsA->vertices, positionVectorA,
                             bsB->vertices, positionVectorB))
            return false;
    }

    return true;
}
