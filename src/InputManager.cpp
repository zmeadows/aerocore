#include "InputManager.hpp"

#include "Base.hpp"
#include "Generator.hpp"
#include "Generator/Bullet.hpp"
#include "Globals.hpp"

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
        case SDLK_LSHIFT: {
            m_keyStates[Key::Shift] = newKeyState;
            keyFunc(this, Key::Shift);
            break;
        }
        default: { break; }
    }
}

void InputManager::processPressedKey(const Key& key) {
    auto CM = get_manager();

    auto& playerCD = CM->get<Entity>(playerUUID());
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
            if (m_keyStates[Key::LeftArrow] != KeyState::Pressed) {
                if (SDL_GetModState() & KMOD_LSHIFT) {
                    playerCD.angvel = -2.5f;
                } else {
                    playerCD.angvel = -7.f;
                }
            }
            break;
        }

        case Key::LeftArrow: {
            if (m_keyStates[Key::RightArrow] != KeyState::Pressed) {
                if (SDL_GetModState() & KMOD_LSHIFT) {
                    playerCD.angvel = 2.5f;
                } else {
                    playerCD.angvel = 7.f;
                }
            }
            break;
        }

        case Key::Spacebar: {

                // CM->book<Alliance>(bulletUUID, Alliance::Friend);
                //
                const v2& player_pos = playerCD.pos;
                const v2& player_vel = playerCD.vel;

                v2 bullet_pos, bullet_vel;

                bullet_pos.x = player_pos.x - 4.f * std::sin(player_angle);
                bullet_pos.y = player_pos.y + 4.f * std::cos(player_angle);

                bullet_vel.x = player_vel.x - 100.f * std::sin(player_angle);
                bullet_vel.y = player_vel.y + 100.f * std::cos(player_angle);

                generateBullet(bullet_pos, bullet_vel);

            break;
        }

        case Key::Shift: {
                             if (fabs(playerCD.angvel) > 0) {
                                 playerCD.angvel = 2.5f * signum(playerCD.angvel);
                             }
                             break;
                         }
    }
}

void InputManager::processReleasedKey(const Key& key) {
    auto CM = get_manager();

    auto& playerCD = CM->get<Entity>(playerUUID());

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
                if (SDL_GetModState() & KMOD_LSHIFT) {
                    playerCD.angvel = 2.5f;
                } else {
                    playerCD.angvel = 7.f;
                }
        } else {
            playerCD.angvel = 0.f;
        }
        break;
    }
    case Key::LeftArrow: {
        if (m_keyStates[Key::RightArrow] == KeyState::Pressed) {
                if (SDL_GetModState() & KMOD_LSHIFT) {
                    playerCD.angvel = -2.5f;
                } else {
                    playerCD.angvel = -7.f;
                }
        } else {
            playerCD.angvel = 0.f;
        }
        break;
    }
    case Key::Spacebar: {
        break;
    }

        case Key::Shift:
                            {
                             if (fabs(playerCD.angvel) > 0) {
                                 playerCD.angvel = 7.f * signum(playerCD.angvel);
                             }
                             break;
                         }
    }
}
