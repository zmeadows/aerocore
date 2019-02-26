#include "System/InvincibilitySystem.hpp"

#include "Component/Common.hpp"
#include "Engine/ComponentManager.hpp"

void InvincibilitySystem::run(ComponentManager* CM, f32 dt) {
    for (const UUID uuid : this->followed) {
        auto& sprite = CM->get<Sprite>(uuid);
        auto& flash = CM->get<Invincibility>(uuid);

        flash.timer -= dt;

        if (flash.timer <= 0.f) {
            if (flash.is_flashing) flash.num_flashes--;
            flash.is_flashing = !flash.is_flashing;
            flash.timer += 0.1;
        }

		if (flash.num_flashes == 0)
            this->finished.append(uuid);
    }

	for (const UUID uuid : this->finished) {
        CM->remove<Invincibility>(uuid);
	}

    this->finished.clear();
}
