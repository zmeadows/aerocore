#include "InputManager.hpp"

#include "Bullet/Bullet.hpp"
#include "Entity/Player.hpp"
#include "Globals.hpp"
#include "Component/Common.hpp"

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

void tick_player(PlayerInput* input, PlayerState* state, f32 dt) {

    auto CM = get_manager();

    if (state->shot_countdown > 0.f)
        state->shot_countdown -= dt;

    const bool player_wants_to_shoot = input->joystick_active[RIGHT_JOYSTICK];

    if (player_wants_to_shoot && state->shot_countdown <= 0.f) {
        generate_player_bullet(input->joystick_state[RIGHT_JOYSTICK]);
        state->shot_countdown += state->shot_delay;
    }

    auto& kin = CM->get<EulerTranslation>(playerUUID());
    if (input->joystick_active[LEFT_JOYSTICK]) {
        kin.vel = 90.f * input->joystick_state[LEFT_JOYSTICK];
    } else {
        kin.vel = { 0.f, 0.f };
    }
}
