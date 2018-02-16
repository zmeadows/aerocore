#pragma once

#include "Base.hpp"
#include "Sprite.hpp"

#include "aerocore.hpp"

#include <SDL2/SDL2_gfxPrimitives.h>

#include <array>
#include <memory>
#include <string>

//TODO: combine translation/rotation systems into MotionSystem
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
        CM->subscribe<Position, Rotation, Sprite, Alliance>(this);
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

    std::array<double, 100> m_fpsHistory;
    Uint64 m_lastFrameTicks;
    size_t m_modFrame;
    double m_currentFpsAvg;

public:
    void run(float) final;

    DrawSystem(ComponentManager* const CM_, GraphicsContext* const GC_)
        : System("Draw", CM_), GC(GC_), m_fpsHistory({{0.0}}), m_lastFrameTicks(SDL_GetPerformanceCounter()),
          m_modFrame(0) {
        CM->subscribe<Sprite, Position, Rotation>(this);
    }
};

//TODO: just remove this class entirely
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
