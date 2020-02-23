#pragma once

#include <SDL.h>
#include <SDL_gpu.h>
// #include <NFont/NFont.h>

#include "Vector2D.hpp"
#include "unstd/types.hpp"

template <s16 LogicalWidth, s16 LogicalHeight, s16 OffsetX, s16 OffsetY>
struct ScreenRegion {

    // Cartesian coordinates with origin directly in the center of the screen region
    struct LocalRealCoordinates {
        f32 x;
        f32 y;

        static const f32 max_x = (f32)LogicalWidth / 2.f;
        static const f32 min_x = -1.f * (f32)LogicalWidth / 2.f;
        static const f32 max_y = (f32)LogicalHeight / 2.f;
        static const f32 min_y = -1.f * (f32)LogicalHeight / 2.f;
    };

    // Discrete coordinates with origin in upper left of ScreenRegion as in SDL convention
    struct LocalPixelCoordinates {
        s16 x;
        s16 y;
    };
};

template <s16 LogicalWidth, s16 LogicalHeight>
struct Screen {
    using Self = Screen<LogicalWidth, LogicalHeight>;

    struct PixelCoordinates {
        s16 x;
        s16 y;

    private:
        Coordinates() = delete;
        Coordinates(s16 x, x16 y) : x(x), y(y) {}
    };

    // struct Span {
    //     const s16 pixels;

    // private:
    //     Span() = delete;
    //     Spawn(s16 pixels) : pixels(pixels) {}
    // };

    template <s16 W, s16 H, s16 X, s16 Y>
    Self::Coordinates convert(ScreenRegion<W, H, X, Y>::LocalRealCoordinates local) {
        static_assert(X + Width <= LogicalWidth && Y + Height <= LogicalHeight);
    }

    template <s16 W, s16 H, s16 X, s16 Y>
    Self::Coordinates convert(ScreenRegion<W, H, X, Y>::LocalPixelCoordinates local) {
        static_assert(X + Width <= LogicalWidth && Y + Height <= LogicalHeight);
    }

    // template <s16 W, s16 H, s16 X, s16 Y>
    // Self::Span distance(ScreenRegion<W, H, X, Y>::LocalRealCoordinates localA,
    //                     ScreenRegion<W, H, X, Y>::LocalRealCoordinates localB) {
    //     static_assert(X + Width <= LogicalWidth && Y + Height <= LogicalHeight);
    // }
};

class GraphicsContext {
    SDL_GameController* gamepad;
    // NFont font;

public:
    GraphicsContext(u16 screen_width, u16 screen_height);
    ~GraphicsContext(void);

    GPU_Target* renderer;
    const u16 screen_width;
    const u16 screen_height;

    ScreenDistance convert_to_screen_distance(f32 game_distance) const;
    ScreenCoordinates convert_to_screen_coords(v2 position) const;
};
