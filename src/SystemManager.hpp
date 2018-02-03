#pragma once

#include "Base.hpp"
#include "System.hpp"

#include "aerocore.hpp"
using namespace aerocore;

#include <SDL2/SDL2_gfxPrimitives.h>

#include <array>
#include <memory>
#include <string>

class TranslationSystem final : public System {
    void wrapEntity(const UUID& uuidA);

public:
    void run(float dt) final;

    TranslationSystem(ComponentManager* const CM_) : System("Translation", CM_) {
        CM->subscribe<Position, Velocity, Acceleration>(this);
    }
};

class RotationSystem final : public System {
public:
    void run(float dt) final;

    RotationSystem(ComponentManager* const CM_) : System("Rotation", CM_) {
        CM->subscribe<Rotation, RotationalVelocity>(this);
    }
};

class CollisionSystem final : public System {
    bool areColliding(const UUID& uuidA, const UUID& uuidB);

public:
    void run(float dt) final;

    CollisionSystem(ComponentManager* const CM_) : System("Collision", CM_) {
        CM->subscribe<Position, Rotation, BoundingSurface, Alliance>(this);
    }
};

class CleanupSystem final : public System {
public:
    void run(float dt) final;

    CleanupSystem(ComponentManager* const CM_) : System("Cleanup", CM_) { CM->subscribe<DeathTimer>(this); }
};

class ParticleSystem final : public System {
public:
    void run(float dt) final;

    ParticleSystem(ComponentManager* const CM_) : System("Particle", CM_) {
        CM->subscribe<ParticleGenerator>(this);
    }
};

class DrawSystem final : public System {
    GraphicsContext* const GC;

    std::array<float, 100> m_fpsHistory;
    Uint64 m_lastFrameTicks;
    size_t m_modFrame;
    float m_currentFpsAvg;

public:
    void run(float dt) final {
        Uint64 tmp = SDL_GetPerformanceCounter();
        m_fpsHistory[m_modFrame % 100] =
            1.0 / ((float)(tmp - m_lastFrameTicks) / SDL_GetPerformanceFrequency());
        m_lastFrameTicks = tmp;
        m_modFrame++;

        for (const UUID& uuid : m_followed) {
            auto spr = CM->get<Sprite>(uuid);

            auto sprUpd = CM->get<SpriteUpdator>(uuid);
            if (sprUpd) {
                sprUpd->update(spr);
            };

            spr->draw(GC, *CM->get<Position>(uuid), *CM->get<Rotation>(uuid));
            // auto bs = CM->get<BoundingSurface>(uuid);
            // if (bs) {
            //     bs->draw(GC, *CM->get<Position>(uuid), *CM->get<Rotation>(uuid));
            // }
        }

        stringColor(GC->renderer, 10, 10, "SCORE: 0", 0xFFFFFFFF);

        if (m_modFrame % 100 == 99) {
            float sum = 0;
            for (size_t i = 0; i < 100; i++) {
                sum += m_fpsHistory[i];
            }
            m_currentFpsAvg = sum / 100.0;
        }

        stringColor(GC->renderer, 735, 10,
                    ("FPS: " + std::to_string(static_cast<int>(std::round(m_currentFpsAvg)))).c_str(),
                    0xFFFFFFFF);
    }

    DrawSystem(ComponentManager* const CM_, GraphicsContext* const GC_)
        : System("Draw", CM_), GC(GC_), m_fpsHistory({{0.0}}), m_lastFrameTicks(SDL_GetPerformanceCounter()),
          m_modFrame(0) {
        CM->subscribe<Sprite, Position, Rotation>(this);
    }
};

class SystemManager {
private:
    std::vector<std::unique_ptr<System>> m_systems;

public:
    SystemManager(void) {}

    SystemManager(const SystemManager&) = delete;
    SystemManager(SystemManager&&) = delete;
    SystemManager& operator=(const SystemManager&) = delete;
    SystemManager& operator=(SystemManager&&) = delete;

    inline void addSystem(System* sys) {
        // TODO: check system with same name hasn't already been added
        m_systems.emplace_back(sys);
    }

    void runSystems(const float dt) {
        for (std::unique_ptr<System>& sys : m_systems) {
            sys->run(dt);
        }
    }
};
