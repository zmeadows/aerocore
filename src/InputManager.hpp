#pragma once

#include <cassert>
#include <functional>
#include <unordered_map>

#include <SDL2/SDL.h>

#include "aerocore.hpp"
#include "QuadTree.hpp"

enum class Key { LeftArrow, RightArrow, UpArrow, DownArrow, Spacebar, Shift };
enum class KeyState { Pressed, Released };

class InputManager {
    std::unordered_map<Key, KeyState> m_keyStates;

    void processPressedKey(const Key& key);
    void processReleasedKey(const Key& key);

public:
    InputManager() {
        m_keyStates[Key::LeftArrow] = KeyState::Released;
        m_keyStates[Key::RightArrow] = KeyState::Released;
        m_keyStates[Key::DownArrow] = KeyState::Released;
        m_keyStates[Key::UpArrow] = KeyState::Released;
        m_keyStates[Key::Spacebar] = KeyState::Released;
    }
    void processInput(SDL_Keycode SDLkey, bool keyUp);
};

