#pragma once

#include <array>
#include <memory>
#include <string>

//TODO: just remove this class entirely
class SystemManager {
private:
    std::vector<std::unique_ptr<System>> m_systems;

public:
    SystemManager(void) {}

    SystemManager(const SystemManager&) = delete;
    SystemManager(SystemManager&&) = delete;
    SystemManager& operator=(const SystemManager&) = delete;
    SystemManager& operator=(SystemManager&&) = delete;

    inline void addSystem(System* sys) {
        // TODO: check system with same name hasn't already been added
        m_systems.emplace_back(sys);
    }

    void runSystems(const float dt) {
        for (std::unique_ptr<System>& sys : m_systems) {
            sys->run(dt);
        }
    }
};
