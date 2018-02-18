#include "InputManager.hpp"

#include "Base.hpp"
#include "Generator.hpp"
#include "Generator/Bullet.hpp"

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
    auto& playerCD = CM->get<CoreData>(playerUUID());
    const float player_angle = playerCD.angle;

    m_keyStates[key] = KeyState::Pressed;

    switch (key) {
        case Key::UpArrow: {
            v2& player_acc = playerCD.acc;
            player_acc.x = -75 * std::sin(player_angle);
            player_acc.y = 75 * std::cos(player_angle);
            break;
        }

        case Key::DownArrow: {
            break;
        }

        case Key::RightArrow: {
            if (m_keyStates[Key::LeftArrow] != KeyState::Pressed)
                playerCD.angvel = -7.f;
            break;
        }

        case Key::LeftArrow: {
            if (m_keyStates[Key::RightArrow] != KeyState::Pressed)
                playerCD.angvel = 7.f;
            break;
        }

        case Key::Spacebar: {
            for (float angle : { -0.5, -0.4, -0.25, -0.15 -0.05, 0.0, 0.05, 0.15, 0.25, 0.4, 0.5 }) {
                const UUID bulletUUID = generateBullet(CM, m_quadTree);
                CM->book<Alliance>(bulletUUID, Alliance::Friend);
                const v2& player_pos = playerCD.pos;
                const v2& player_vel = playerCD.vel;

                auto& bulletCD = CM->get<CoreData>(bulletUUID);

                bulletCD.pos.x = player_pos.x - 7.5f * std::sin(player_angle);
                bulletCD.pos.y = player_pos.y + 7.5f * std::cos(player_angle);

                bulletCD.vel.x = 0.5f * player_vel.x - 125.f * std::sin(player_angle + angle);
                bulletCD.vel.y = 0.5f * player_vel.y + 125.f * std::cos(player_angle + angle);
            }

            break;
        }
    }
}

void InputManager::processReleasedKey(const Key& key) {
    auto& playerCD = CM->get<CoreData>(playerUUID());

    m_keyStates[key] = KeyState::Released;

    switch (key) {

    case Key::UpArrow: {
        playerCD.acc.x = 0;
        playerCD.acc.y = 0;
        break;
    }
    case Key::DownArrow: {
        break;
    }
    case Key::RightArrow: {
        if (m_keyStates[Key::LeftArrow] == KeyState::Pressed) {
            playerCD.angvel = 7;
        } else {
            playerCD.angvel = 0.f;
        }
        break;
    }
    case Key::LeftArrow: {
        if (m_keyStates[Key::RightArrow] == KeyState::Pressed) {
            playerCD.angvel = -7.f;
        } else {
            playerCD.angvel = 0.f;
        }
        break;
    }
    case Key::Spacebar: {
        break;
    }
    }
}
