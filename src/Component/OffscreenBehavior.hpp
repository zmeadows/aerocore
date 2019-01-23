#pragma once

#include "unstd/types.hpp"

struct OffscreenBehavior {
    enum class Type {
        DiesInstantly,
        SinglePassAllowed,
        ValidRange,
        Wraps
    } type = Type::DiesInstantly;

    union {
        struct {
            f32 minX = -250.f;
            f32 maxX = 250.f;
            f32 minY = -250.f;
            f32 maxY = 250.f;
        } ValidRange;

        struct {
            bool already_found_onscreen = false;
        } SinglePassAllowed;
    };

    OffscreenBehavior(void) {}
};





