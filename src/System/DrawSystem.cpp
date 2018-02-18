#include "System/DrawSystem.hpp"
#include "Geometry.hpp"

void DrawSystem::run(float) {
    Uint64 tmp = SDL_GetPerformanceCounter();
    m_fpsHistory[m_modFrame % 100] =
        1.0 / (static_cast<double>(tmp - m_lastFrameTicks) / static_cast<double>(SDL_GetPerformanceFrequency()));
    m_lastFrameTicks = tmp;
    m_modFrame++;

    for (const UUID uuid : m_followed) {
        const CoreData& CD = CM->get<CoreData>(uuid);

        // if (CM->has<SpriteUpdator>(uuid)) {
        //     CM->get<SpriteUpdator>(uuid).update(spr);
        // };



        if (CM->has<CollisionData>(uuid) && m_modFrame % 100 < 50) {
            SDL_Color test;
            test.r = 255;
            test.g = 255;
            test.b = 255;
            test.a = 255;
            for (const auto& tri : CM->get<CollisionData>(uuid).triangles) {
                draw(GC, tri, CD.pos, CD.angle, test);
            }
        } else {
         draw(GC, CD.vertices, CD.pos, CD.angle, CD.color);
        }

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

    // stringColor(GC->renderer, 735, 10,
    //             ("FPS: " + std::to_string(static_cast<int>(std::round(m_currentFpsAvg)))).c_str(),
    //             0xFFFFFFFF);
}
