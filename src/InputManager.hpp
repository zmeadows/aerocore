#pragma once

#include <cassert>

#include "ComponentManager.hpp"
#include "UUID.hpp"
#include "Generator.hpp"


class InputManager {
    ComponentManager* const CM;

public:
    InputManager(ComponentManager* const CM_) : CM(CM_) {}
    void processInput(SDL_Keycode key, bool keyUp);
};

void InputManager::processInput(SDL_Keycode key, bool keyUp) {
  auto vel = CM->get<Velocity>(UUID::playerUUID);

  assert(vel != nullptr);

  if (!keyUp) {
    switch (key) {
    case SDLK_UP: {
      vel->y = -1;
      break;
    }
    case SDLK_DOWN: {
      vel->y = 1;
      break;
    }
    case SDLK_RIGHT: {
      vel->x = 1;
      break;
    }
    case SDLK_LEFT: {
      vel->x = -1;
      break;
    }
    case SDLK_SPACE: {
      UUID bulletUUID = generate<EntityType::Bullet>(CM);
      auto playerPos = CM->get<Position>(UUID::playerUUID);

      CM->add<Position>(bulletUUID, {playerPos->x, playerPos->y - 25});
      CM->add<Velocity>(bulletUUID, {0, -1});
      break;
    }
    default: { break; }
    }
  } else {
    switch (key) {
    case SDLK_UP:
      vel->y = 0;
      break;
    case SDLK_DOWN:
      vel->y = 0;
      break;
    case SDLK_RIGHT:
      vel->x = 0;
      break;
    case SDLK_LEFT:
      vel->x = 0;
      break;
    case SDLK_SPACE:
      break;
    default:
      break;
    }
  }
}
