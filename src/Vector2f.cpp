#include "Vector2D.hpp"

v2 operator+(const v2& vecA, const v2& vecB) {
    return {vecA.x + vecB.x, vecA.y + vecB.y};
}

v2 operator-(const v2& vecA, const v2& vecB) {
    return {vecA.x - vecB.x, vecA.y - vecB.y};
}

float v2::dot(const v2& rhs) const {
    return x * rhs.x + y * rhs.y;
}

void v2::rotate(float angle) {
    const float tmpCos = std::cos(angle);
    const float tmpSin = std::sin(angle);
    const float px = tmpCos * this->x - tmpSin * this->y;
    const float py = tmpSin * this->x + tmpCos * this->y;
    this->x = px;
    this->y = py;
}

void v2::scale(float factor) {
    this->x *= factor;
    this->y *= factor;
}

v2 v2::rotated(float angle) const {
    v2 tmp = *this;
    tmp.rotate(angle);
    return tmp;
}

float v2::magnitude(void) const {
    return std::sqrtf(x * x + y * y);
}

v2 v2::normalized(void) const {
    v2 tmp = *this;
    tmp.scale(1.f / tmp.magnitude());
    return tmp;
}

float cross(const v2& lhs, const v2& rhs) {
    return lhs.x * rhs.y - lhs.y * rhs.x;
}

float distance(const v2& lhs, const v2& rhs) {
    return std::sqrt( std::pow(lhs.x - rhs.x, 2.f) + std::pow(lhs.y - rhs.y, 2.f) );
}

bool operator==(const v2& vecA, v2& vecB) { return vecA.x == vecB.x && vecA.y == vecB.y; }
bool operator==(v2& vecA, const v2& vecB) { return vecA.x == vecB.x && vecA.y == vecB.y; }
bool operator==(const v2& vecA, const v2& vecB) { return vecA.x == vecB.x && vecA.y == vecB.y; }


