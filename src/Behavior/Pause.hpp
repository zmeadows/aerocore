#pragma once

#include "Globals.hpp"
#include "Component/StateTransition.hpp"

struct PauseBehavior {
    f32 time_left;
    s32 next_state_id;
};

class PauseSystem final : public System {
public:
    void run(float dt) final {
        auto CM = get_manager();

        std::vector<UUID> finished_entities;

        for (const UUID uuid : m_followed) {
            auto& pause = CM->get<PauseBehavior>(uuid);

            pause.time_left -= dt;

            if (pause.time_left <= 0.f) {
                auto& transition = CM->book<StateTransition>(uuid);
                transition.next_state_id = pause.next_state_id;
                transition.extra_time = -1 * pause.time_left;

                finished_entities.push_back(uuid);
            }
        }

        for (const UUID uuid : finished_entities) {
            CM->remove<PauseBehavior>(uuid);
        }
    }

    PauseSystem(void) : System("Pause")
    {
        get_manager()->subscribe<PauseBehavior>(this);
    }
};
