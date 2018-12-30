#include "System/OffscreenBehaviorSystem.hpp"

#include "Entity.hpp"

void OffscreenBehaviorSystem::run(float) {
    ComponentManager* CM = get_manager();

    for (const UUID uuid : m_followed) {
        auto& entity = CM->get<Entity>(uuid);
        const Extent& ext = entity.extent;

        if (!is_offscreen(ext)) {
            auto& osb = CM->get<OffscreenBehavior>(uuid);

            if (osb.type == OffscreenBehavior::Type::SinglePassAllowed)
                osb.SinglePassAllowed.already_found_onscreen = true;

            continue;
        }

        const auto& kin = CM->get<EulerTranslation>(uuid);
        const auto& osb = CM->get<OffscreenBehavior>(uuid);

        switch (osb.type) {
            case OffscreenBehavior::Type::DiesInstantly: {
                CM->book<DestructTag>(uuid);
                break;
            }

            case OffscreenBehavior::Type::Wraps: {
                auto& update = CM->get<PositionUpdate>(uuid);
                if (ext.maxX > 100.f && kin.vel.x > 0.f) {
                    update.delta.x -= (ext.maxX + 100.f);
                }

                if (ext.minX < -100.f && kin.vel.x < 0.f) {
                    update.delta.x -= (ext.minX - 100.f);
                }

                if (ext.maxY > 100.f && kin.vel.y > 0.f) {
                    update.delta.y -= (ext.maxY + 100.f);
                }

                if (ext.minY < -100.f && kin.vel.y < 0.f) {
                    update.delta.y -= (ext.minY - 100.f);
                }
                break;
            }

            case OffscreenBehavior::Type::SinglePassAllowed: {
                if (osb.SinglePassAllowed.already_found_onscreen) {
                    CM->book<DestructTag>(uuid);
                }
                break;
            }

            case OffscreenBehavior::Type::ValidRange: {
                if (  entity.pos.x > osb.ValidRange.maxX
                    || entity.pos.x < osb.ValidRange.minX
                    || entity.pos.y > osb.ValidRange.maxY
                    || entity.pos.y < osb.ValidRange.minY) {
                    CM->book<DestructTag>(uuid);
                }
                break;
            }
        }
    }
}
