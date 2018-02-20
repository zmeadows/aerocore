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

SurfaceNormalSet::SurfaceNormalSet(const std::vector<v2>& vertices,
                                   const std::vector<std::vector<size_t>>& triangle_indices)
{
    for (size_t j = 0; j < triangle_indices.size(); j++) {
        const size_t numVertices = triangle_indices[j].size();
        for (size_t i = 0; i < numVertices; i++) {
            const size_t idx1 = triangle_indices[j][i];
            const size_t idx2 = triangle_indices[j][(i + 1) % numVertices];
            v2 ov = vertices[idx2] - vertices[idx1];
            std::swap(ov.x, ov.y);
            if (ov.x != 0.f)
                ov.x *= -1.0;
            add(ov);
        }
    }
}

SurfaceNormalSet& SurfaceNormalSet::operator=(const SurfaceNormalSet& other) {
    // check for self-assignment
    if(&other == this)
        return *this;
    this->normals = other.normals;
    return *this;
}

SurfaceNormalSet::SurfaceNormalSet(const SurfaceNormalSet& rhs) {
    normals.reserve(rhs.size());
    for (size_t i = 0; i < rhs.normals.size(); i++) {
        this->add(rhs.normals[i]);
    }
}

SurfaceNormalSet::SurfaceNormalSet(SurfaceNormalSet&& rhs) {
    std::swap(this->normals, rhs.normals);
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
    const Extent extA = entityA.extent;
    const Extent extB = entityB.extent;

    // @REMINDER: currently collisions are not processed offscreen (could change)
    if (is_offscreen(extA) || is_offscreen(extB))
        return false;

    // first check if the AABB's overlap, since this offers an early exit in many cases.
    if (extA.minX > extB.maxX || extB.minX > extB.maxX)
        return false;
    if (extA.minY > extB.maxY || extB.minY > extB.maxY)
        return false;

    for (const std::vector<size_t>& trA : colA.triangle_indices) {
        for (const std::vector<size_t>& trB : colB.triangle_indices) {
            bool these_triangles_separated = false;

            for (const v2& axis : colA.normals) {
                AxisProjection projA = project_on(entityA.global_vertices, trA, axis);
                AxisProjection projB = project_on(entityB.global_vertices, trB, axis);
                if (!(projA.max >= projB.min && projB.max >= projA.min)) {
                    these_triangles_separated = true;
                    break;
                }
            }

            if (these_triangles_separated)
                continue;

            for (const v2& axis : colB.normals) {
                AxisProjection projA = project_on(entityA.global_vertices, trA, axis);
                AxisProjection projB = project_on(entityB.global_vertices, trB, axis);
                if (!(projA.max >= projB.min && projB.max >= projA.min)) {
                    these_triangles_separated = true;
                    break;
                }
            }

            if (!these_triangles_separated) return true;
        }
    }

    return false;
}

AxisProjection
project_on(const std::vector<v2>& global_vertices, const std::vector<size_t>& indices, const v2& axis) {
    float minProjection = std::numeric_limits<float>::max();
    float maxProjection = std::numeric_limits<float>::lowest();

    for (const size_t idx : indices) {
        const float projection = global_vertices[idx].dot(axis);
        minProjection = std::min(projection, minProjection);
        maxProjection = std::max(projection, maxProjection);
    }

    return AxisProjection({minProjection, maxProjection});
}

CollisionData::CollisionData(const Entity& entity)
    : triangle_indices(decompose_into_triangle_indices(entity.local_vertices))
    , normals(SurfaceNormalSet(entity.local_vertices, triangle_indices))
{ }
