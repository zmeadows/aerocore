#include "System/DrawSystem.hpp"

#include "Component/Common.hpp"
#include "Globals.hpp"

DrawSystem::DrawSystem() :
    System("Draw"),
    m_fpsHistory(),
    m_lastFrameTicks(SDL_GetPerformanceCounter()),
    m_modFrame(0)
{
    get_manager()->subscribe<Entity, Sprite>(this);
}

void DrawSystem::run(float) {
    // Uint64 tmp = SDL_GetPerformanceCounter();
    // m_fpsHistory[m_modFrame % 100] =
    //     1.0 / (static_cast<double>(tmp - m_lastFrameTicks) / static_cast<double>(SDL_GetPerformanceFrequency()));
    // m_lastFrameTicks = tmp;
    // m_modFrame++;

    auto CM = get_manager();
    auto GC = get_graphics_context();

    // draw background
    GPU_ClearRGB(GC->renderer, 20, 20, 20);
    GPU_CircleFilled(GC->renderer,
                     GC->to_screen_span(100.f),
                     GC->to_screen_span(100.f),
                     100,
                     { 40,40,40,255});

    // drawQuadTree(GC, get_quad_tree());

    for (const UUID uuid : m_followed) {
        const Entity& entity = CM->get<Entity>(uuid);
        const Sprite& sprite = CM->get<Sprite>(uuid);

        ScreenCoordinates sc = GC->to_screen_coords(entity.pos);
        const v2 offset = sprite.offset.rotated(-entity.angle);
        GPU_BlitRotate(sprite.image, NULL, GC->renderer,
                       sc.x - offset.x, sc.y - offset.y,
                       - entity.angle * 180.f/PI);
    }

    // stringColor(GC->renderer, 10, 10, "SCORE: 0", 0xFFFFFFFF);

    // if (m_modFrame % 100 == 99) {
    //     double sum = 0;
    //     for (size_t i = 0; i < 100; i++) {
    //         sum += m_fpsHistory[i];
    //     }
    //     m_currentFpsAvg = sum / 100.0;
    //     std::cout << "average fps: " << m_currentFpsAvg << std::endl;
    // }


    // stringColor(GC->renderer, 735, 10,
    //             ("FPS: " + std::to_string(static_cast<int>(std::round(m_currentFpsAvg)))).c_str(),
    //             0xFFFFFFFF);
}

