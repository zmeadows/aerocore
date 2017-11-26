#include "InputManager.hpp"

#include "ComponentManager.hpp"
#include "Generator.hpp"

void InputManager::processInput(SDL_Keycode SDLkey, bool keyUp)
{
    const KeyState newKeyState = keyUp ? KeyState::Released : KeyState::Pressed;
    auto keyFunc = keyUp ? std::mem_fn(&InputManager::processReleasedKey)
                         : std::mem_fn(&InputManager::processPressedKey);

    DEBUG("received input: " << SDLkey);

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
    default: {
        break;
    }
    }
}

void InputManager::processPressedKey(const Key& key)
{
    auto acc = CM->get<Acceleration>(UUID::playerUUID);
    assert(acc != nullptr);

    m_keyStates[key] = KeyState::Pressed;

    switch (key) {

    case Key::UpArrow: {
        if (m_keyStates[Key::DownArrow] != KeyState::Pressed)
            acc->y = 500;
        break;
    }
    case Key::DownArrow: {
        if (m_keyStates[Key::UpArrow] != KeyState::Pressed)
            acc->y = -500;
        break;
    }
    case Key::RightArrow: {
        if (m_keyStates[Key::LeftArrow] != KeyState::Pressed)
            acc->x = 500;
        break;
    }
    case Key::LeftArrow: {
        if (m_keyStates[Key::RightArrow] != KeyState::Pressed)
            acc->x = -500;
        break;
    }
    case Key::Spacebar: {
        UUID bulletUUID = generate<EntityType::Bullet>(CM);
        CM->add<Alliance>(bulletUUID, Alliance::Friend);
        auto const playerPos = CM->get<Position>(UUID::playerUUID);
        auto pos = CM->get<Position>(bulletUUID);
        pos->x = playerPos->x;
        pos->y = playerPos->y + 5;
        break;
    }
    default: {
        break;
    }
    }
}

void InputManager::processReleasedKey(const Key& key)
{
    auto acc = CM->get<Acceleration>(UUID::playerUUID);
    assert(acc != nullptr);

    m_keyStates[key] = KeyState::Released;

    switch (key) {

    case Key::UpArrow: {
        if (m_keyStates[Key::DownArrow] == KeyState::Pressed) {
            acc->y = -500;
        } else {
            acc->y = 0;
        }
        break;
    }
    case Key::DownArrow: {
        if (m_keyStates[Key::UpArrow] == KeyState::Pressed) {
            acc->y = 500;
        } else {
            acc->y = 0;
        }
        break;
    }
    case Key::RightArrow: {
        if (m_keyStates[Key::LeftArrow] == KeyState::Pressed) {
            acc->x = -500;
        } else {
            acc->x = 0;
        }
        break;
    }
    case Key::LeftArrow: {
        if (m_keyStates[Key::RightArrow] == KeyState::Pressed) {
            acc->x = 500;
        } else {
            acc->x = 0;
        }
        break;
    }
    case Key::Spacebar: {
        break;
    }
    default: {
        break;
    }
    }
}