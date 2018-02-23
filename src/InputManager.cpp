#include "InputManager.hpp"

#include "Base.hpp"
#include "Generator.hpp"
#include "Generator/Bullet.hpp"
#include "Globals.hpp"
#include "Entity.hpp"

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
            player_acc.x = 75 * std::cos(player_angle);
            player_acc.y = 75 * std::sin(player_angle);
            break;
        }

        case Key::DownArrow: {
            if (m_keyStates[Key::UpArrow] != KeyState::Pressed) {
                v2& player_acc = playerCD.acc;
                player_acc.x = -75 * std::cos(player_angle);
                player_acc.y = -75 * std::sin(player_angle);
            }
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
                const v2& player_orientation = orientation_of(playerCD);

                v2 bullet_pos, bullet_vel;

                v2 bullet_pos_offset = player_orientation;
                bullet_pos_offset.scale(4.f);

                bullet_pos = player_pos + bullet_pos_offset;
                bullet_vel = orientation_of(playerCD);
                bullet_vel.scale(100.f);

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

    // don't 'get' this every frame, just keep it stored.
    auto& playerCD = CM->get<Entity>(playerUUID());

    m_keyStates[key] = KeyState::Released;

    switch (key) {

    case Key::UpArrow: {
        v2& player_acc = playerCD.acc;
        const float player_angle = playerCD.angle;
        if (m_keyStates[Key::DownArrow] != KeyState::Pressed) {
            playerCD.acc.x = 0;
            playerCD.acc.y = 0;
        } else {
            player_acc.x = -75 * std::cos(player_angle);
            player_acc.y = -75 * std::sin(player_angle);
        }
        break;
    }
    case Key::DownArrow: {
        v2& player_acc = playerCD.acc;
        const float player_angle = playerCD.angle;
        if (m_keyStates[Key::UpArrow] != KeyState::Pressed) {
            playerCD.acc.x = 0;
            playerCD.acc.y = 0;
        } else {
            player_acc.x = 75 * std::cos(player_angle);
            player_acc.y = 75 * std::sin(player_angle);
        }
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
