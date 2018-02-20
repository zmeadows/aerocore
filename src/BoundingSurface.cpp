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

//@FIXME: This is still inefficient. Move individual triangles into quad tree
//and only test surface normals from each pair of triangles.
bool overlaps(const CollisionData& colA, const Entity& entityA,
              const CollisionData& colB, const Entity& entityB)
{
    const Extent extA = extent_of(entityA);
    const Extent extB = extent_of(entityB);

    // @REMINDER: currently collisions are not processed offscreen (could change)
    if (is_offscreen(extA) || is_offscreen(extB))
        return false;

    // first check if the AABB's overlap, since this offers an early exit in many cases.
    if (extA.minX > extB.maxX || extB.minX > extB.maxX)
        return false;
    if (extA.minY > extB.maxY || extB.minY > extB.maxY)
        return false;

    // combine surface normals into one SurfaceNormalSet so that
    // duplicates are removed, rather than separately looping over both sets
    SurfaceNormalSet combinedSurfaceNormals;
    combinedSurfaceNormals.add(colA.normals, entityA.angle);
    combinedSurfaceNormals.add(colB.normals, entityB.angle);

    for (const std::vector<v2>& trA : colA.triangles) {
        for (const std::vector<v2>& trB : colB.triangles) {
            bool collided = true;
            for (const v2& axis : combinedSurfaceNormals) {
                AxisProjection projA = project_on(trA, axis, entityA.pos, entityA.angle);
                AxisProjection projB = project_on(trB, axis, entityB.pos, entityB.angle);
                if (!(projA.max >= projB.min && projB.max >= projA.min)) {
                    collided = false;
                    break;
                }
            }
            if (collided) return true;
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
