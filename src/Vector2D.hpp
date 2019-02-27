#pragma once

#include "unstd/types.hpp"
#include <cmath>
#include "Util.hpp"

namespace {

const f32 VEC2D_COMPARISON_DELTA = 1e-6f;

inline float arctan(float x, float y) {
    return fmod(atan2(y,x) + TWOPI, TWOPI);
}

inline bool rough_equals(f32 x, f32 y, f32 delta) {
    return fabs(x - y) < delta;
}

}

struct v2 {
    f32 x = 0.f;
    f32 y = 0.f;

    inline v2 normalized(void) const {
        v2 tmp = *this;
        const f32 m = tmp.magnitude();
        if (m > 0) tmp.scale(1.f / tmp.magnitude());
        return tmp;
    }

    inline f32 dot(const v2& rhs) const {
        return x * rhs.x + y * rhs.y;
    }

    inline f32 magnitude(void) const {
        return sqrtf(x * x + y * y);
    }

    inline void rotate(f32 angle) {
        const f32 tmpCos = std::cos(angle);
        const f32 tmpSin = std::sin(angle);
        this->x = tmpCos * this->x - tmpSin * this->y;
        this->y = tmpSin * this->x + tmpCos * this->y;
    }

    inline v2 rotated(f32 angle) const {
        v2 tmp(*this);
        tmp.rotate(angle);
        return tmp;
    }

    inline void scale(f32 factor) {
        this->x *= factor;
        this->y *= factor;
    }

    inline f32 angle(void) const {
        return arctan(x,y);
    }

    inline v2& operator+=(const v2& rhs) {
      this->x += rhs.x;
      this->y += rhs.y;
      return *this;
    }
};

inline v2 operator/(const v2& vec, f32 scale) {
    return {vec.x / scale, vec.y / scale};
}

inline v2 operator*(const v2& vec, f32 scale) {
    return {vec.x * scale, vec.y * scale};
}

inline v2 operator*(f32 scale, const v2& vec) {
    return {vec.x * scale, vec.y * scale};
}

inline bool operator==(const v2& vecA, const v2& vecB) {
    return rough_equals(vecA.x, vecB.x, VEC2D_COMPARISON_DELTA)
        && rough_equals(vecA.y, vecB.y, VEC2D_COMPARISON_DELTA);
}

inline bool operator==(const v2& vecA, v2& vecB) {
    return rough_equals(vecA.x, vecB.x, VEC2D_COMPARISON_DELTA)
        && rough_equals(vecA.y, vecB.y, VEC2D_COMPARISON_DELTA);
}

inline bool operator==(v2& vecA, const v2& vecB) {
    return rough_equals(vecA.x, vecB.x, VEC2D_COMPARISON_DELTA)
        && rough_equals(vecA.y, vecB.y, VEC2D_COMPARISON_DELTA);
}

inline v2 operator+(const v2& vecA, const v2& vecB) {
    return {vecA.x + vecB.x, vecA.y + vecB.y};
}

inline v2 operator-(const v2& vecA, const v2& vecB) {
    return {vecA.x - vecB.x, vecA.y - vecB.y};
}


inline f32 cross(const v2& lhs, const v2& rhs) {
    return lhs.x * rhs.y - lhs.y * rhs.x;
}

inline f32 distance(const v2& lhs, const v2& rhs) {
    const f32 dx = lhs.x - rhs.x;
    const f32 dy = lhs.y - rhs.y;
    return std::sqrt( dx*dx + dy*dy );
}
