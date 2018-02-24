#include "System/DrawSystem.hpp"
#include "Geometry.hpp"
#include "BoundingSurface.hpp"

void DrawSystem::run(float) {
    Uint64 tmp = SDL_GetPerformanceCounter();
    m_fpsHistory[m_modFrame % 100] =
        1.0 / (static_cast<double>(tmp - m_lastFrameTicks) / static_cast<double>(SDL_GetPerformanceFrequency()));
    m_lastFrameTicks = tmp;
    m_modFrame++;

    auto CM = get_manager();
    auto GC = get_graphics_context();

    for (const UUID uuid : m_followed) {
        const Entity& CD = CM->get<Entity>(uuid);

        // if (m_modFrame % 100 < 50) {
        //     SDL_Color test;
        //     test.r = 255;
        //     test.g = 0;
        //     test.b = 255;
        //     test.a = 255;
        //     const PolygonDecomposition& pd = CD.poly_decomp;
        //     for (size_t pgon = 0; pgon < pd.count; pgon++) {
        //         PolygonRep pr = nth_polygon(pd, pgon);
        //         const size_t vtx_count = static_cast<size_t>(pr.count);

        //         for (size_t i = 0; i < vtx_count; i++) {
        //             ScreenCoordinates sc1 = GC->to_screen_coords(CD.global_vertices[pr.indices[i]]);
        //             ScreenCoordinates sc2 = GC->to_screen_coords(CD.global_vertices[pr.indices[(i+1) % vtx_count]]);

        //             GPU_Line(GC->renderer,
        //                      sc1.x, sc1.y,
        //                      sc2.x, sc2.y,
        //                      CD.color);
        //         }
        //     }
        // } else {
            draw(*GC, CD);
        // }

        //TODO: add switch to turn on/off bounding surfaces with key press in debug mode.
        // auto bs = CM->get<BoundingSurface>(uuid);
        // if (bs) {
        //     bs->draw(GC, *CM->get<Position>(uuid), *CM->get<Rotation>(uuid));
        // }
    }

    // stringColor(GC->renderer, 10, 10, "SCORE: 0", 0xFFFFFFFF);

    if (m_modFrame % 100 == 99) {
        double sum = 0;
        for (size_t i = 0; i < 100; i++) {
            sum += m_fpsHistory[i];
        }
        m_currentFpsAvg = sum / 100.0;
    }

    stringColor(GC->renderer, 735, 10,
                ("FPS: " + std::to_string(static_cast<int>(std::round(m_currentFpsAvg)))).c_str(),
                0xFFFFFFFF);
}
