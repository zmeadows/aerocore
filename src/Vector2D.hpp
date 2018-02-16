#pragma once

#include <cmath>

struct v2 {
    float x = 0.f;
    float y = 0.f;

    v2 normalized(void) const;
    v2 rotated(float angle) const;
    float dot(const v2& rhs) const;
    float magnitude(void) const;
    void rotate(float angle);
    void scale(float factor);
};

v2 operator+(const v2& vecA, const v2& vecB);

v2 operator-(const v2& vecA, const v2& vecB);

