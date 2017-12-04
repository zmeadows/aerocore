#include "Geometry.hpp"

// float PolygonShape::area(void) const
// {
//     float sum = 0;
//     const size_t numVertices = vertices.size();
//
//     const auto det = [](const Vector2f& v1, const Vector2f& v2) -> float {
//         return v1.x() * v2.y() - v1.y() * v2.x();
//     };
//
//     for (size_t i = 0; i < numVertices; i++) {
//         sum += det(vertices[i], vertices[(i + 1) % numVertices]);
//     }
//
//     return 0.5 * sum;
// }

void PolygonShape::scale(float factor) {
    for (Vector2f& vtx : this->vertices)
        vtx.scale(factor);
}

std::vector<Vector2f> PolygonShape::getTransRotVertices(const Position& pos, const Rotation& rot) const {
    std::vector<Vector2f> tmpVertices;
    tmpVertices.reserve(this->vertices.size());
    const Vector2f posVec = {pos.x, pos.y};

    for (const Vector2f& vtx : this->vertices) {
        tmpVertices.push_back(vtx.rotated(rot.getAngle()) + posVec);
    }

    return tmpVertices;
}