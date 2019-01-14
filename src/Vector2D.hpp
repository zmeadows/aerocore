#pragma once

#include "Typedef.hpp"

struct v2 {
    f32 x = 0.f;
    f32 y = 0.f;

    v2 normalized(void) const;
    v2 rotated(f32 angle) const;
    f32 dot(const v2& rhs) const;
    f32 magnitude(void) const;
    void rotate(f32 angle);
    void scale(f32 factor);
    f32 angle(void) const;
    v2& operator+=(const v2& rhs);
};

v2 operator+(const v2& vecA, const v2& vecB);
v2 operator-(const v2& vecA, const v2& vecB);

v2 operator*(const v2& vec, f32 scale);
v2 operator*(f32 scale, const v2& vec);

v2 operator/(const v2& vec, f32 scale);

bool operator==(const v2& vecA, const v2& vecB);
bool operator==(const v2& vecA, v2& vecB);
bool operator==(v2& vecA, const v2& vecB);

f32 cross(const v2& lhs, const v2& rhs);
f32 distance(const v2& lhs, const v2& rhs);
