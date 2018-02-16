#pragma once

#include <cmath>

struct Vector2f {
    float x = 0.f;
    float y = 0.f;

    Vector2f normalized(void) const;
    Vector2f rotated(float angle) const;
    float dot(const Vector2f& rhs) const;
    float magnitude(void) const;
    void rotate(float angle);
    void scale(float factor);
};

Vector2f operator+(const Vector2f& vecA, const Vector2f& vecB);

Vector2f operator-(const Vector2f& vecA, const Vector2f& vecB);

