#include "InputManager.hpp"

#include "Base.hpp"
#include "Generator/Bullet.hpp"
#include "Generator/Player.hpp"
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

    auto& player = CM->get<Entity>(playerUUID());
    auto& kin    = CM->get<EulerTranslation>(playerUUID());
    auto& rot    = CM->get<EulerRotation>(playerUUID());

    m_keyStates[key] = KeyState::Pressed;

    switch (key) {
        case Key::UpArrow: {
            kin.acc.x = 75 * std::cos(player.angle);
            kin.acc.y = 75 * std::sin(player.angle);
            break;
        }

        case Key::DownArrow: {
            if (m_keyStates[Key::UpArrow] != KeyState::Pressed) {
                kin.acc.x = -75 * std::cos(player.angle);
                kin.acc.y = -75 * std::sin(player.angle);
            }
            break;
        }

        case Key::RightArrow: {
            if (m_keyStates[Key::LeftArrow] != KeyState::Pressed) {
                if (SDL_GetModState() & KMOD_LSHIFT) {
                    rot.vel = -2.5f;
                } else {
                    rot.vel = -7.f;
                }
            }
            break;
        }

        case Key::LeftArrow: {
            if (m_keyStates[Key::RightArrow] != KeyState::Pressed) {
                if (SDL_GetModState() & KMOD_LSHIFT) {
                    rot.vel = 2.5f;
                } else {
                    rot.vel = 7.f;
                }
            }
            break;
        }

        case Key::Spacebar:
            generatePlayerBullet();
            break;

        case Key::Shift: {
            if (fabs(rot.vel) > 0) {
                rot.vel = 2.5f * signum(rot.vel);
            }
            break;
        }
    }
}

void InputManager::processReleasedKey(const Key& key) {
    auto CM = get_manager();

    auto& player = CM->get<Entity>(playerUUID());
    auto& kin    = CM->get<EulerTranslation>(playerUUID());
    auto& rot    = CM->get<EulerRotation>(playerUUID());

    m_keyStates[key] = KeyState::Released;

    switch (key) {

        case Key::UpArrow: {
            if (m_keyStates[Key::DownArrow] != KeyState::Pressed) {
                kin.acc.x = 0;
                kin.acc.y = 0;
            } else {
                kin.acc.x = -75 * std::cos(player.angle);
                kin.acc.y = -75 * std::sin(player.angle);
            }
            break;
        }

        case Key::DownArrow: {
            if (m_keyStates[Key::UpArrow] != KeyState::Pressed) {
                kin.acc.x = 0;
                kin.acc.y = 0;
            } else {
                kin.acc.x = 75 * std::cos(player.angle);
                kin.acc.y = 75 * std::sin(player.angle);
            }
            break;
        }

        case Key::RightArrow: {
            if (m_keyStates[Key::LeftArrow] == KeyState::Pressed) {
                    if (SDL_GetModState() & KMOD_LSHIFT) {
                        rot.vel = 2.5f;
                    } else {
                        rot.vel = 7.f;
                    }
            } else {
                rot.vel = 0.f;
            }
            break;
        }

        case Key::LeftArrow: {
            if (m_keyStates[Key::RightArrow] == KeyState::Pressed) {
                    if (SDL_GetModState() & KMOD_LSHIFT) {
                        rot.vel = -2.5f;
                    } else {
                        rot.vel = -7.f;
                    }
            } else {
                rot.vel = 0.f;
            }
            break;
        }

        case Key::Shift: {
            if (fabs(rot.vel) > 0) {
                rot.vel = 7.f * signum(rot.vel);
            }
            break;
        }

        case Key::Spacebar: break;
    }
}

void InputManager::processJoystickInput(void) {
    auto CM = get_manager();

    auto& kin = CM->get<EulerTranslation>(playerUUID());

    kin.vel = 75.f * this->joystick;
}

void InputManager::processGamepadButtonInput(Uint8 button, bool press) {
    switch (button) {
        case 0:
            generatePlayerBullet();
            break;
    }
}
