#include "System/PauseSystem.hpp"

#include "Globals.hpp"
#include "Component/Common.hpp"

void run(PauseSystem& self, f32 dt) {
    auto CM = get_manager();

    for (const UUID uuid : self.base.followed) {
        auto& pause = CM->get<PauseBehavior>(uuid);

        pause.time_left -= dt;

        if (pause.time_left <= 0.f) {
            auto& transition = CM->book<StateTransition>(uuid);
            transition.next_state_id = pause.next_state_id;
            transition.extra_time = -1 * pause.time_left;

            append(self.finished, uuid);
        }
    }

    for (const UUID uuid : self.finished) {
        CM->remove<PauseBehavior>(uuid);
    }

    clear(self.finished);
}
