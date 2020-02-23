#include "Coordinates.hpp"



Position operator*(Velocity vel, TimeDelta dt) {
    return { vel.x * dt.value, vel.y * dt.value };
}
Position operator*(TimeDelta dt, Velocity vel) {
    return vel * dt;
}

Velocity operator*(Acceleration, TimeDelta);
Velocity operator*(TimeDelta, Acceleration);

const f32 Position::max_x = 16.f / 2.f;
const f32 Position::max_y = 9.f / 2.f;
const f32 Position::min_x = -1.f * Position::max_x;
const f32 Position::min_y = -1.f * Position::max_y;
