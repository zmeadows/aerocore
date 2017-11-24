#pragma once

#include "Base.hpp"
#include "ComponentManager.hpp"
#include "System.hpp"

class MovementSystem final : public System {
public:
    void run(float dt) final {
        for (const UUID& uuid : m_followed) {
            auto pos = CM->get<Position>(uuid);
            auto const vel = CM->get<Velocity>(uuid);
            pos->x += dt * vel->x;
            pos->y += dt * vel->y;
        }
    }

    MovementSystem(ComponentManager* const CM_) : System("Movement", CM_) {
        CM->subscribe<Position, Velocity>(this);
    }
};

class CollisionSystem final : public System {
public:
    void run(float dt) final {
        std::set<std::pair<UUID, UUID>> collisions;

        for (std::set<UUID>::const_iterator uuidA = m_followed.begin();
             uuidA != m_followed.end(); ++uuidA) {

            for (std::set<UUID>::const_iterator uuidB = next(uuidA);
                 uuidB != m_followed.end(); ++uuidB) {

                const auto allA = CM->get<Alliance>(*uuidA);
                const auto allB = CM->get<Alliance>(*uuidB);

                if (*allA != *allB) {

                    const auto bsA = CM->get<BoundingSurface>(*uuidA);
                    const auto posA = CM->get<Position>(*uuidA);
                    const auto bsB = CM->get<BoundingSurface>(*uuidB);
                    const auto posB = CM->get<Position>(*uuidB);

                    if (overlaps(bsA, posA, bsB, posB)) {
                        collisions.insert(std::minmax(*uuidA, *uuidB));
                        DEBUG("COLLIDED");
                    }
                }
            }
        }
    }

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

    DrawSystem(ComponentManager* const CM_, GraphicsContext* const GC_)
        : System("Draw", CM_), GC(GC_) {
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
          m_collision(std::make_unique<CollisionSystem>(CM)),
          m_draw(std::make_unique<DrawSystem>(CM, GC)) {}

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