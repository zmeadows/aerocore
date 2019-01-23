#pragma once

#include "unstd/types.hpp"

struct StateTransition {
    s32 next_state_id = -1;
    f32 extra_time = 0.0;
};
