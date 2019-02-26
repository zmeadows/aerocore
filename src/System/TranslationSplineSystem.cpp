#include "System/TranslationSplineSystem.hpp"

#include "Engine/ComponentManager.hpp"
#include "Spline.hpp"
#include "Component/Common.hpp"

void TranslationSplineSystem::run(ComponentManager* CM, f32 dt) {
    for (const UUID uuid : this->followed) {
        auto& spline = CM->get<TranslationSpline>(uuid);
        auto& update = CM->get<PositionUpdate>(uuid);
        const auto& entity = CM->get<Entity>(uuid);

        SplineStepResult<v2> result = spline.step(dt);

        update.delta = result.new_point - entity.pos;

        if (result.finished) {
            this->finished.append(uuid);
            if (spline.next_state_id >= 0) {
                auto& transition = CM->book<StateTransition>(uuid);
                transition.next_state_id = spline.next_state_id;
                transition.extra_time = result.leftover_dt;
            }
        }
    }

    for (const UUID uuid : this->finished) {
        CM->remove<TranslationSpline>(uuid);
    }

    this->finished.clear();
}
