#pragma once

#include <cassert>
#include <functional>
#include <unordered_map>

#include <SDL2/SDL.h>

class ComponentManager;

enum class Key { LeftArrow, RightArrow, UpArrow, DownArrow, Spacebar };
enum class KeyState { Pressed, Released };

class InputManager {
    ComponentManager* const CM;

    std::unordered_map<Key, KeyState> m_keyStates;

    void processPressedKey(const Key& key);
    void processReleasedKey(const Key& key);

public:
    InputManager(ComponentManager* const CM_) : CM(CM_) {
        m_keyStates[Key::LeftArrow] = KeyState::Released;
        m_keyStates[Key::RightArrow] = KeyState::Released;
        m_keyStates[Key::DownArrow] = KeyState::Released;
        m_keyStates[Key::UpArrow] = KeyState::Released;
        m_keyStates[Key::Spacebar] = KeyState::Released;
    }
    void processInput(SDL_Keycode SDLkey, bool keyUp);
};
