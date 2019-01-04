#include "System/LinearPathSystem.hpp"

#include "Entity.hpp"

void LinearPathSystem::run(float dt) {
    ComponentManager* CM = get_manager();

    for (const UUID uuid : m_followed) {
        auto& entity  = CM->get<Entity>(uuid);
        auto& lpath  = CM->get<LinearPath>(uuid);
        auto& update = CM->get<PositionUpdate>(uuid);

        lpath.t_elapsed += dt;

        v2 point_on_path = lpath.from + (lpath.to - lpath.from) * (lpath.t_elapsed / lpath.t_total);

        update.delta = point_on_path - entity.pos;
    }
}
