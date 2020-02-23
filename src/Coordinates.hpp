#pragma once

#include "unstd/types.hpp"

#include <cmath>

struct TimeDelta {
    const f32 value;
    TimeDelta(f32 dt) : value(dt) {}
};

struct GlobalWindowCoordinates {
    s16 x = -999;
    s16 y = -999;

    static s16 window_width;
    static s16 window_height;

    GlobalWindowCoordinates operator+(const GlobalWindowCoordinates& other) const {
        return {this->x + other.x, this->y + other.y};
    }

    GlobalWindowCoordinates operator-(const GlobalWindowCoordinates& other) const {
        return {this->x - other.x, this->y - other.y};
    }
};

struct LocalWindowRegion {
    const s16 minX;
    const s16 maxX;
    const s16 minY;
    const s16 maxY;

    LocalWindowRegion(s16 minX, s16 maxX, s16 minY, s16 maxY)
        : minX(minX), maxX(maxX), minY(minY), maxY(maxY) {
        assert(minX < maxX);
        assert(minY < maxY);
    }
};

template <f32 MinX, f32 MaxX, f32 MinY, f32 MaxY>
struct BoundedRealCoordinates {
    using Self = BoundedRealCoordinates<MinX, MaxX, MinY, MaxY>;

    f32 x = 0.f;
    f32 y = 0.f;

    bool out_of_bounds() const { return x < MinX || x > MaxX || y < MinY || y > MaxX; };

    Self operator+(const Self& other) const {
        MyClass result = *this;
        result.x += other.x;
        result.y += other.y;
        return result;
    }

    Self operator-(const Self& other) const {
        MyClass result = *this;
        result.x -= other.x;
        result.y -= other.y;
        return result;
    }

    GlobalWindowCoordinates to_global_from_local(LocalWindowRegion region) const {
        const f32 region_width = static_cast<f32>(region.maxX - region.minX);
        const f32 region_height = static_cast<f32>(region.maxY - region.minY);

        const f32 sf_x = region_width / MaxX;
        const f32 sf_y = region_height / MinY;

        const s16 region_x = static_cast<s16>(nearbyint(sf_x * this->x + region_width / 2.f));
        const s16 region_y = static_cast<s16>(nearbyint(sf_y * this->y + region_height / 2.f));

        const s16 screen_x =
    }
};

