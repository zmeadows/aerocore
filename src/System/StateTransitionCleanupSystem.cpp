#include "System/StateTransitionCleanupSystem.hpp"

#include "Component/StateTransition.hpp"
#include "Globals.hpp"

#include "Brofiler.h"

StateTransitionCleanupSystem::StateTransitionCleanupSystem() : System("StateTransitionCleanup")
{
    get_manager()->subscribe<StateTransition>(this);
}

void StateTransitionCleanupSystem::run(float dt) {
	BROFILER_CATEGORY("StateTransitionCleanupSystem", Profiler::Color::DarkRed)

    auto CM = get_manager();

    //TODO: make this a member variable
    std::vector<UUID> followed_copy;

    for (const UUID uuid : m_followed) {
        followed_copy.push_back(uuid);
    }

    for (const UUID uuid : followed_copy) {
        CM->remove<StateTransition>(uuid);
    }
}
