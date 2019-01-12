#pragma once

#include "Globals.hpp"

struct StateTransition {
    s32 next_state_id = -1;
    f32 extra_time = 0.0;
};

class StateTransitionCleanupSystem final : public System {
public:
    void run(float dt) final {
        auto CM = get_manager();

        std::vector<UUID> followed_copy;

        for (const UUID uuid : m_followed) {
            followed_copy.push_back(uuid);
        }

        for (const UUID uuid : followed_copy) {
            CM->remove<StateTransition>(uuid);
        }
	}

    StateTransitionCleanupSystem() : System("StateTransitionCleanup")
	{
        get_manager()->subscribe<StateTransition>(this);
    }
};

