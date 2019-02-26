#include "InputManager.hpp"

#include "Bullet/Bullet.hpp"
#include "Component/Common.hpp"
#include "Engine/ComponentManager.hpp"
#include "Entity/Player.hpp"

namespace {

const f32 JOYSTICK_DEAD_ZONE = 0.1f;

}

void process_input(PlayerInput* input, ButtonPress press) {
    input->button_state[press.button] = true;

    switch (press.button) {
        default: break;
    }
}

void process_input(PlayerInput* input, ButtonRelease release) {
    input->button_state[release.button] = false;

    switch (release.button) {
        default: break;
    }
}

void process_input(PlayerInput* input, JoystickMotion motion) {

    if (motion.axis == X_AXIS) {
        input->joystick_state[motion.which].x = motion.value;
    } else {
        assert(motion.axis == Y_AXIS);
        input->joystick_state[motion.which].y = motion.value;
    }

    input->joystick_active[motion.which] =
        input->joystick_state[motion.which].magnitude() > JOYSTICK_DEAD_ZONE;
}

void process_input(PlayerInput* input, TriggerMotion motion) {
    return;
}

