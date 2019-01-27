#include "System/InvincibilitySystem.hpp"

#include "Component/Common.hpp"
#include "Globals.hpp"

InvincibilitySystem::InvincibilitySystem() : System("Invincibility") {
    get_manager()->subscribe<Sprite, Invincibility>(this);
}

void InvincibilitySystem::run(f32 dt) {
    auto CM = get_manager();
    std::vector<UUID> finished_uuids;

    for (const UUID uuid : m_followed) {
        auto& sprite = CM->get<Sprite>(uuid);
        auto& flash = CM->get<Invincibility>(uuid);

        flash.timer -= dt;

        if (flash.timer <= 0.f) {
            if (flash.is_flashing) flash.num_flashes--;
            flash.is_flashing = !flash.is_flashing;
            flash.timer += 0.1;
        }

		if (flash.num_flashes == 0)
            finished_uuids.push_back(uuid);
    }

	for (const UUID uuid : finished_uuids) {
        CM->remove<Invincibility>(uuid);
	}
}
