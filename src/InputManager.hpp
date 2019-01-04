#pragma once

#include <cassert>
#include <functional>
#include <unordered_map>
#include <iostream>

#include <SDL.h>

#include "Vector2D.hpp"

enum class Key { LeftArrow, RightArrow, UpArrow, DownArrow, Spacebar, Shift };
enum class KeyState { Pressed, Released };

class InputManager {
    std::unordered_map<Key, KeyState> m_keyStates;

    void processPressedKey(const Key& key);
    void processReleasedKey(const Key& key);

    v2 joystick;

public:
    InputManager() {
        m_keyStates[Key::LeftArrow]  = KeyState::Released;
        m_keyStates[Key::RightArrow] = KeyState::Released;
        m_keyStates[Key::DownArrow]  = KeyState::Released;
        m_keyStates[Key::UpArrow]    = KeyState::Released;
        m_keyStates[Key::Spacebar]   = KeyState::Released;
    }

    void processInput(SDL_Keycode SDLkey, bool keyUp);

    void processJoystickInput(void);

    void processGamepadButtonInput(Uint8 button, bool press);

    void updateGamepadJoystickX(Sint16 x) {
        joystick.x = static_cast<float>(x) / 32768.f;
    }

    void updateGamepadJoystickY(Sint16 y) {
        joystick.y = - static_cast<float>(y) / 32768.f;
    }
};

