#pragma once

#include "Typedef.hpp"
#include "Vector2D.hpp"
#include "Entity/Player.hpp"

enum JoystickID {
    LEFT_JOYSTICK = 0,
    RIGHT_JOYSTICK,
};

enum JoystickAxis {
    X_AXIS = 0,
    Y_AXIS,
};

enum GamepadButton {
    A_BUTTON = 0,
    B_BUTTON,
    X_BUTTON,
    Y_BUTTON,
    RIGHT_BUMPER,
    LEFT_BUMPER
};

enum GamepadTrigger {
    RIGHT_TRIGGER = 0,
    LEFT_TRIGGER
};

struct JoystickMotion {
    JoystickID which;
    JoystickAxis axis;
    f32 value;
};

struct ButtonPress {
    GamepadButton button;
};

struct ButtonRelease {
    GamepadButton button;
};

struct TriggerMotion {
    GamepadTrigger which;
    f32 value;
};

struct PlayerInput {
    v2 joystick_state[2]      = { {0.f, 0.f}, {0.f, 0.f} };
    bool joystick_active[2] = { false, false };
    f32 trigger_state[2]      = { 0.f, 0.f };
    bool button_state[6]      = { false, false, false, false, false, false };
};

void process_input(PlayerInput* input, ButtonPress button);
void process_input(PlayerInput* input, ButtonRelease button);
void process_input(PlayerInput* input, JoystickMotion button);
void process_input(PlayerInput* input, TriggerMotion button);

void tick_player(PlayerInput* input, PlayerState* state, f32 dt);
