#include "InputManager.hpp"

#include "Generator.hpp"

void InputManager::processInput(SDL_Keycode SDLkey, bool keyUp) {
    const KeyState newKeyState = keyUp ? KeyState::Released : KeyState::Pressed;
    auto keyFunc = keyUp ? std::mem_fn(&InputManager::processReleasedKey)
                         : std::mem_fn(&InputManager::processPressedKey);

    switch (SDLkey) {
    case SDLK_UP: {
        m_keyStates[Key::UpArrow] = newKeyState;
        keyFunc(this, Key::UpArrow);
        break;
    }
    case SDLK_DOWN: {
        m_keyStates[Key::DownArrow] = newKeyState;
        keyFunc(this, Key::DownArrow);
        break;
    }
    case SDLK_RIGHT: {
        m_keyStates[Key::RightArrow] = newKeyState;
        keyFunc(this, Key::RightArrow);
        break;
    }
    case SDLK_LEFT: {
        m_keyStates[Key::LeftArrow] = newKeyState;
        keyFunc(this, Key::LeftArrow);
        break;
    }
    case SDLK_SPACE: {
        m_keyStates[Key::Spacebar] = newKeyState;
        keyFunc(this, Key::Spacebar);
        break;
    }
    default: { break; }
    }
}

void InputManager::processPressedKey(const Key& key) {
    auto& acc = CM->get<Acceleration>(playerUUID());
    auto& rotVel = CM->get<RotationalVelocity>(playerUUID());

    m_keyStates[key] = KeyState::Pressed;

    switch (key) {
        case Key::UpArrow: {
            const float angle = CM->get<Rotation>(playerUUID()).getAngle();
            acc.x = -75 * std::sin(angle);
            acc.y = 75 * std::cos(angle);
            break;
        }

        case Key::DownArrow: {
            break;
        }

        case Key::RightArrow: {
            if (m_keyStates[Key::LeftArrow] != KeyState::Pressed)
                rotVel.value = -7;
            break;
        }

        case Key::LeftArrow: {
            if (m_keyStates[Key::RightArrow] != KeyState::Pressed)
                rotVel.value = 7;
            break;
        }

        case Key::Spacebar: {
            UUID bulletUUID = generate<EntityType::Bullet>(CM);
            CM->book<Alliance>(bulletUUID, Alliance::Friend);
            const auto& playerPos = CM->get<Position>(playerUUID());
            const auto& playerVel = CM->get<Velocity>(playerUUID());
            const float angle = CM->get<Rotation>(playerUUID()).getAngle();

            auto& pos = CM->get<Position>(bulletUUID);
            pos.x = playerPos.x - 7.5f * std::sin(angle);
            pos.y = playerPos.y + 7.5f * std::cos(angle);

            auto& vel = CM->get<Velocity>(bulletUUID);
            vel.x = 0.5f * playerVel.x - 125.f * std::sin(angle);
            vel.y = 0.5f * playerVel.y + 125.f * std::cos(angle);

            break;
        }
    }
}

void InputManager::processReleasedKey(const Key& key) {
    auto& acc = CM->get<Acceleration>(playerUUID());
    auto& rotVel = CM->get<RotationalVelocity>(playerUUID());

    m_keyStates[key] = KeyState::Released;

    switch (key) {

    case Key::UpArrow: {
        acc.x = 0;
        acc.y = 0;
        break;
    }
    case Key::DownArrow: {
        break;
    }
    case Key::RightArrow: {
        if (m_keyStates[Key::LeftArrow] == KeyState::Pressed) {
            rotVel.value = 7;
        } else {
            rotVel.value = 0.f;
        }
        break;
    }
    case Key::LeftArrow: {
        if (m_keyStates[Key::RightArrow] == KeyState::Pressed) {
            rotVel.value = -7;
        } else {
            rotVel.value = 0.f;
        }
        break;
    }
    case Key::Spacebar: {
        break;
    }
    }
}
