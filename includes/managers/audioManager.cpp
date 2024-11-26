//
// Created by ginko on 17/04/24.
//

#include "audioManager.h"
#include "assets.h"


// TODO make a copy for each sound?
namespace Audio {
    void Play(const std::string &key) {
        if (const auto sound = Assets::GetSound(key); IsSoundPlaying(sound))
            PlaySound(LoadSoundAlias(sound));
            // TODO this should probably be preloaded or at least limited (there could be infinite sounds)
        else
            PlaySound(sound);
    }

    void Update(entt::registry& registry) {
        registry.group<Audio::Command>().each([&registry](auto entity, auto &command) {
            Audio::Play(command.soundName);
            registry.destroy(entity);
        });
    }
}


// TODO this needs to be attached to an event system: everytime the player attacks this has to trigger updateAudio and updateAnimation and so on
//void updateAudio(entt::registry &registry) {
//    playerView = registry.view<Player,
//}
