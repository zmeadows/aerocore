#pragma once

#include "Base.hpp"
#include "ComponentManager.hpp"
#include "System.hpp"

class MovementSystem final : public System {
public:
    void run(float dt) final;

    MovementSystem(ComponentManager* const CM_) : System("Movement", CM_) {
        CM->subscribe<Position, Velocity, Acceleration>(this);
    }
};

class CollisionSystem final : public System {
    bool areColliding(const UUID& uuidA, const UUID& uuidB);

public:
    void run(float dt) final;

    CollisionSystem(ComponentManager* const CM_) : System("Collision", CM_) {
        CM->subscribe<Position, BoundingSurface, Alliance>(this);
    }
};

class DrawSystem final : public System {
    GraphicsContext* const GC;

public:
    void run(float dt) final {
        for (const UUID& uuid : m_followed) {
            CM->get<Sprite>(uuid)->draw(GC, *CM->get<Position>(uuid));
        }
    }

    DrawSystem(ComponentManager* const CM_, GraphicsContext* const GC_) : System("Draw", CM_), GC(GC_) {
        CM->subscribe<Sprite, Position>(this);
    }
};

class SystemManager {
private:
    std::unique_ptr<MovementSystem> m_movement;
    std::unique_ptr<CollisionSystem> m_collision;
    std::unique_ptr<DrawSystem> m_draw;

public:
    SystemManager(ComponentManager* const CM, GraphicsContext* const GC)
        : m_movement(std::make_unique<MovementSystem>(CM)),
          m_collision(std::make_unique<CollisionSystem>(CM)), m_draw(std::make_unique<DrawSystem>(CM, GC)) {}

    SystemManager(const SystemManager&) = delete;
    SystemManager(SystemManager&&) = delete;
    SystemManager& operator=(const SystemManager&) = delete;
    SystemManager& operator=(SystemManager&&) = delete;

    void runSystems(const float dt) {
        m_movement->run(dt);
        m_collision->run(dt);
        m_draw->run(dt);
    }
};