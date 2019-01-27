#include "System/TranslationSplineSystem.hpp"

#include "Globals.hpp"
#include "Spline.hpp"
#include "Component/Common.hpp"

void run(TranslationSplineSystem& self, f32 dt) {
    auto CM = get_manager();

    for (const UUID uuid : self.base.followed) {
        auto& spline = CM->get<TranslationSpline>(uuid);
        auto& update = CM->get<PositionUpdate>(uuid);
        const auto& entity = CM->get<Entity>(uuid);

        SplineStepResult<v2> result = spline.step(dt);

        update.delta = result.new_point - entity.pos;

        if (result.finished) {
            append(self.finished, uuid);
            if (spline.next_state_id >= 0) {
                auto& transition = CM->book<StateTransition>(uuid);
                transition.next_state_id = spline.next_state_id;
                transition.extra_time = result.leftover_dt;
            }
        }
    }

    for (const UUID uuid : self.finished) {
        CM->remove<TranslationSpline>(uuid);
    }

    clear(self.finished);
}
