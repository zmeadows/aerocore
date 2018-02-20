#pragma once

#include "Base.hpp"
#include "Sprite.hpp"
#include "Globals.hpp"

#include "aerocore.hpp"

#include <array>
#include <memory>
#include <string>


class CleanupSystem final : public System {
public:
    void run(float dt) final;

    CleanupSystem() : System("Cleanup") {
        get_manager()->subscribe<DeathTimer>(this);
    }
};

class ParticleSystem final : public System {
public:
    void run(float dt) final;

    ParticleSystem() : System("Particle") {
        get_manager()->subscribe<ParticleGenerator>(this);
    }
};


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
