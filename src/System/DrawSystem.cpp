#include "Entity.hpp"
#include "System/DrawSystem.hpp"
#include "QuadTreeDraw.hpp"
#include "Sprite.hpp"

void draw_background(void) {
    GraphicsContext* GC = get_graphics_context();

    GPU_ClearRGB(GC->renderer, 40, 40, 40);
    GPU_CircleFilled(GC->renderer,
                     GC->to_screen_span(100.f),
                     GC->to_screen_span(100.f),
                     100,
                     { 219,147,47,255});
}

void DrawSystem::run(float) {
    Uint64 tmp = SDL_GetPerformanceCounter();
    m_fpsHistory[m_modFrame % 100] =
        1.0 / (static_cast<double>(tmp - m_lastFrameTicks) / static_cast<double>(SDL_GetPerformanceFrequency()));
    m_lastFrameTicks = tmp;
    m_modFrame++;

    auto CM = get_manager();
    auto GC = get_graphics_context();
    draw_background();
    //drawQuadTree(GC, get_quad_tree());

    for (const UUID uuid : m_followed) {
        const Entity& entity = CM->get<Entity>(uuid);
        const Sprite& sprite = CM->get<Sprite>(uuid);

        draw(*GC, sprite, entity.pos, entity.angle);

        // if (CM->has<CollisionData>(uuid) && m_modFrame % 100 < 50) {
        //     const auto& CD = CM->get<CollisionData>(uuid);
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
        //                      test);
        //         }
        //     }
        // }

    }

    // stringColor(GC->renderer, 10, 10, "SCORE: 0", 0xFFFFFFFF);

    if (m_modFrame % 100 == 99) {
        double sum = 0;
        for (size_t i = 0; i < 100; i++) {
            sum += m_fpsHistory[i];
        }
        m_currentFpsAvg = sum / 100.0;
        std::cout << m_currentFpsAvg << std::endl;
    }


    // stringColor(GC->renderer, 735, 10,
    //             ("FPS: " + std::to_string(static_cast<int>(std::round(m_currentFpsAvg)))).c_str(),
    //             0xFFFFFFFF);
}
