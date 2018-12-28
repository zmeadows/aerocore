#include "System/SoundSystem.hpp"

#include "Entity.hpp"
#include "Globals.hpp"

void SoundSystem::run(float dt) {
    static ComponentManager* CM = get_manager();
    static AudioContext* AC = get_audio_context();

    for (const UUID uuid : m_followed) {
        const auto& event = CM->get<SoundEvent>(uuid);
        AC->play(event.effect);
        CM->book<DestructTag>(uuid);
    }
}
