#pragma once

#include "Base.hpp"
#include "System.hpp"
#include "ComponentManager.hpp"

class MovementSystem final : public System {
private:
  void runInternal(const UUID& uuid) final {
    auto pos = CM->get<Position>(uuid);
    auto const vel = CM->get<Velocity>(uuid);


    pos->x += vel->x;
    pos->y += vel->y;
  }
public:
  MovementSystem(ComponentManager* const CM_) : System("Movement", CM_) {
    CM->subscribe<Position, Velocity>(this);
  }
};

class CollisionSystem final : public System {
  void runInternal(const UUID& uuid) final {}
public:
  CollisionSystem(ComponentManager* const CM_) : System("Collision", CM_) {
    CM->subscribe<Position>(this);
  }
};

class DrawSystem final : public System {
  SDL_Renderer* const renderer;

  void runInternal(const UUID &uuid) final {
    auto const pos = CM->get<Position>(uuid);
    CM->get<Sprite>(uuid)->draw(renderer, pos->x, pos->y);
  }

public:
  DrawSystem(ComponentManager *const CM_, SDL_Renderer *const renderer_)
      : System("Draw", CM_), renderer(renderer_) {
    CM->subscribe<Sprite>(this);
    CM->subscribe<Position>(this);
  }
};

class SystemManager {
private:
  ComponentManager* const CM;

  std::unique_ptr<MovementSystem> m_movement;
  std::unique_ptr<CollisionSystem> m_collision;
  std::unique_ptr<DrawSystem> m_draw;

public:
  SystemManager(ComponentManager* const CM_, SDL_Renderer* const renderer_)
      : CM(CM_),
        m_movement(std::make_unique<MovementSystem>(CM_)),
        m_collision(std::make_unique<CollisionSystem>(CM_)),
        m_draw(std::make_unique<DrawSystem>(CM_, renderer_)) {}

  SystemManager(const SystemManager &) = delete;
  SystemManager(SystemManager &&) = delete;
  SystemManager &operator=(const SystemManager &) = delete;
  SystemManager &operator=(SystemManager &&) = delete;

  void runSystems(void) {
    m_movement->run();
    m_collision->run();
    m_draw->run();
  }

};