#pragma once

#include "Base.hpp"
#include "ComponentManager.hpp"
#include "System.hpp"

#include <SDL2/SDL2_gfxPrimitives.h>

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

class DrawSystem final : public System {
    GraphicsContext* const GC;

public:
    void run(float dt) final {
        for (const UUID& uuid : m_followed) {
            CM->get<Sprite>(uuid)->draw(GC, *CM->get<Position>(uuid), *CM->get<Rotation>(uuid));
        }

        stringColor(GC->renderer, 10, 10, "SCORE: 0", 0xFFFFFFFF);
    }

    DrawSystem(ComponentManager* const CM_, GraphicsContext* const GC_) : System("Draw", CM_), GC(GC_) {
        CM->subscribe<Sprite, Position, Rotation>(this);
    }
};

class SystemManager {
private:
    std::unique_ptr<TranslationSystem> m_translation;
    std::unique_ptr<RotationSystem> m_rotation;
    std::unique_ptr<CollisionSystem> m_collision;
    std::unique_ptr<DrawSystem> m_draw;

public:
    SystemManager(ComponentManager* const CM, GraphicsContext* const GC)
        : m_translation(std::make_unique<TranslationSystem>(CM)),
          m_rotation(std::make_unique<RotationSystem>(CM)),
          m_collision(std::make_unique<CollisionSystem>(CM)), m_draw(std::make_unique<DrawSystem>(CM, GC)) {}

    SystemManager(const SystemManager&) = delete;
    SystemManager(SystemManager&&) = delete;
    SystemManager& operator=(const SystemManager&) = delete;
    SystemManager& operator=(SystemManager&&) = delete;

    void runSystems(const float dt) {
        m_translation->run(dt);
        m_rotation->run(dt);
        m_collision->run(dt);
        m_draw->run(dt);
    }
};