#pragma once

#include "Engine/System.hpp"
#include "Component/BulletStream.hpp"
#include "Component/Common.hpp"

class AudioContext;
class SpriteCache;

struct BulletStreamSystem final : System {
    AudioContext* AC;
    SpriteCache* SC;
    BulletStreamSystem(AudioContext* _AC, SpriteCache* _SC)
        : AC(_AC), SC(_SC), System("BulletStream") {}
    void run(ComponentManager* CM, float dt);
    DynamicArray<UUID> finished;
    SUBSCRIBE(Entity, BulletStream);
};

