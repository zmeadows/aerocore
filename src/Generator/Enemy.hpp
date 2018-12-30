#pragma once

void generateStabber(void);

struct Stabber {
    enum class State {
        Stabbing,
        Relocating,
        Stopping,
        Aiming
    } state = State::Stopping;

    float speed = 0.f;

    v2 idle_point;
};
