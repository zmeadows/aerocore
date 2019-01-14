#include "Vector2D.hpp"
#include "Util.hpp"

#include <cmath>

namespace {
const f32 VEC2D_COMPARISON_DELTA = 1e-6f;

const float TWOPI = 2.f * static_cast<float>(3.1415926535897932384);

inline float arctan(float x, float y) {
    return fmod(atan2(y,x) + TWOPI, TWOPI);
}

}

//@TODO: check that LLVM is vectorizing these functions when possible

v2 operator/(const v2& vec, f32 scale) {
    return {vec.x / scale, vec.y / scale};
}

v2 operator*(const v2& vec, f32 scale) {
    return {vec.x * scale, vec.y * scale};
}

v2 operator*(f32 scale, const v2& vec) {
    return {vec.x * scale, vec.y * scale};
}

bool operator==(const v2& vecA, const v2& vecB) {
    return rough_equals(vecA.x, vecB.x, VEC2D_COMPARISON_DELTA)
        && rough_equals(vecA.y, vecB.y, VEC2D_COMPARISON_DELTA);
}

bool operator==(const v2& vecA, v2& vecB) {
    return rough_equals(vecA.x, vecB.x, VEC2D_COMPARISON_DELTA)
        && rough_equals(vecA.y, vecB.y, VEC2D_COMPARISON_DELTA);
}

bool operator==(v2& vecA, const v2& vecB) {
    return rough_equals(vecA.x, vecB.x, VEC2D_COMPARISON_DELTA)
        && rough_equals(vecA.y, vecB.y, VEC2D_COMPARISON_DELTA);
}

v2 operator+(const v2& vecA, const v2& vecB) {
    return {vecA.x + vecB.x, vecA.y + vecB.y};
}

v2 operator-(const v2& vecA, const v2& vecB) {
    return {vecA.x - vecB.x, vecA.y - vecB.y};
}

f32 v2::dot(const v2& rhs) const {
    return x * rhs.x + y * rhs.y;
}

void v2::rotate(f32 angle) {
    const f32 tmpCos = std::cos(angle);
    const f32 tmpSin = std::sin(angle);
    const f32 px = tmpCos * this->x - tmpSin * this->y;
    const f32 py = tmpSin * this->x + tmpCos * this->y;
    this->x = px;
    this->y = py;
}

void v2::scale(f32 factor) {
    this->x *= factor;
    this->y *= factor;
}

v2 v2::rotated(f32 angle) const {
    v2 tmp = *this;
    tmp.rotate(angle);
    return tmp;
}

f32 v2::magnitude(void) const {
    return std::sqrtf(x * x + y * y);
}

v2 v2::normalized(void) const {
    v2 tmp = *this;
    const f32 m = tmp.magnitude();
    if (m > 0) tmp.scale(1.f / tmp.magnitude());
    return tmp;
}

f32 cross(const v2& lhs, const v2& rhs) {
    return lhs.x * rhs.y - lhs.y * rhs.x;
}

//@TODO: use fast sqrt? Or is it irrelevant with modern processors?
f32 distance(const v2& lhs, const v2& rhs) {
    const f32 dx = lhs.x - rhs.x;
    const f32 dy = lhs.y - rhs.y;
    return std::sqrt( dx*dx + dy*dy );
}

f32 v2::angle(void) const {
    return arctan(x,y);
}

v2& v2::operator+=(const v2& rhs){
  this->x += rhs.x;
  this->y += rhs.y;
  return *this;
}


