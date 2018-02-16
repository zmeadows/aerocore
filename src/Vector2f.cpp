#include "Vector2D.hpp"

Vector2f operator+(const Vector2f& vecA, const Vector2f& vecB) {
    return {vecA.x + vecB.x, vecA.y + vecB.y};
}

Vector2f operator-(const Vector2f& vecA, const Vector2f& vecB) {
    return {vecA.x - vecB.x, vecA.y - vecB.y};
}

float Vector2f::dot(const Vector2f& rhs) const {
    return x * rhs.x + y * rhs.y;
}

void Vector2f::rotate(float angle) {
    const float tmpCos = std::cos(angle);
    const float tmpSin = std::sin(angle);
    const float px = tmpCos * this->x - tmpSin * this->y;
    const float py = tmpSin * this->x + tmpCos * this->y;
    this->x = px;
    this->y = py;
}

void Vector2f::scale(float factor) {
    this->x *= factor;
    this->y *= factor;
}

Vector2f Vector2f::rotated(float angle) const {
    Vector2f tmp = *this;
    tmp.rotate(angle);
    return tmp;
}

float Vector2f::magnitude(void) const {
    return std::sqrtf(x * x + y * y);
}

Vector2f Vector2f::normalized(void) const {
    Vector2f tmp = *this;
    tmp.scale(1.f / tmp.magnitude());
    return tmp;
}
