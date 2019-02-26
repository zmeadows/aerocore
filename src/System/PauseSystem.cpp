#include "System/PauseSystem.hpp"

#include "Engine/ComponentManager.hpp"
#include "Component/Common.hpp"

void PauseSystem::run(ComponentManager* CM, f32 dt) {
    for (const UUID uuid : this->followed) {
        auto& pause = CM->get<PauseBehavior>(uuid);

        pause.time_left -= dt;

        if (pause.time_left <= 0.f) {
            auto& transition = CM->book<StateTransition>(uuid);
            transition.next_state_id = pause.next_state_id;
            transition.extra_time = -1 * pause.time_left;

            this->finished.append(uuid);
        }
    }

    for (const UUID uuid : this->finished) {
        CM->remove<PauseBehavior>(uuid);
    }

    this->finished.clear();
}
