#pragma once

struct v2 {
    float x = 0.f;
    float y = 0.f;

    v2 normalized(void) const;
    v2 rotated(float angle) const;
    float dot(const v2& rhs) const;
    float magnitude(void) const;
    void rotate(float angle);
    void scale(float factor);
    float angle(void);
    v2& operator+=(const v2& rhs);
};

v2 operator+(const v2& vecA, const v2& vecB);
v2 operator-(const v2& vecA, const v2& vecB);


v2 operator*(const v2& vec, float scale);
v2 operator*(float scale, const v2& vec);

bool operator==(const v2& vecA, const v2& vecB);
bool operator==(const v2& vecA, v2& vecB);
bool operator==(v2& vecA, const v2& vecB);

float cross(const v2& lhs, const v2& rhs);
float distance(const v2& lhs, const v2& rhs);
