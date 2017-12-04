#pragma once

#include "Base.hpp"
#include "Vector2D.hpp"

#include <vector>

struct Shape {
    virtual void scale(float factor) = 0;

    virtual ~Shape(void) {}
};

class CircleShape final : public Shape {
    float radius;

public:
    CircleShape(float radius_) : radius(radius_) {}

    void scale(float factor) final { radius = std::sqrt(factor) * radius; }

    ~CircleShape() {}
};

class PolygonShape : public Shape {
    // in coordinate system with origin at geometrical center of polygon
    std::vector<Vector2f> vertices;

public:
    PolygonShape(std::vector<Vector2f>&& vertices_) { std::swap(vertices, vertices_); }
    PolygonShape(const PolygonShape& rhs) { vertices = rhs.vertices; }

    const std::vector<Vector2f>& getVertices(void) const { return vertices; }
    std::vector<Vector2f> getTransRotVertices(const Position& pos, const Rotation& rot) const;

    void scale(float factor) final;

    virtual ~PolygonShape() {}
};

class IsoTriangleShape : public PolygonShape {
public:
    const float baseWidth;
    const float height;

    IsoTriangleShape(float baseWidth_, float height_)
        : PolygonShape({{-baseWidth_ / 2.f, -height_ / 3.f},
                        {baseWidth_ / 2.f, -height_ / 3.f},
                        {0.f, 2.f * height_ / 3.f}})

          ,
          baseWidth(baseWidth_), height(height_) {}

    ~IsoTriangleShape() {}
};

class SquareShape : public PolygonShape {
public:
    const float width;

    SquareShape(float width_)
        : PolygonShape({{-width_ / 2.f, -width_ / 2.f},
                        {width_ / 2.f, -width_ / 2.f},
                        {width_ / 2.f, width_ / 2.f},
                        {-width_ / 2.f, width_ / 2.f}}),
          width(width_) {}

    ~SquareShape() {}
};
