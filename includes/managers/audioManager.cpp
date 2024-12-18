//
// Created by ginko on 17/04/24.
//

#include "audioManager.h"
#include "assets.h"
#include "managers/game.h"
// TODO this is actually the audio system, it's not 'managing' anything at all
// TODO make a copy for each sound?
namespace Audio {
    void Play(const std::string &key) {
        if (const auto sound = Assets::GetSound(key); IsSoundPlaying(sound))
            PlaySound(LoadSoundAlias(sound)); // TODO this is leaking, find a workaround
//             TODO this should probably be preloaded or at least limited (there could be infinite sound
        else
            PlaySound(sound);

//        auto sound = Assets::GetSound(key);
    }

    void Update() {
        Game::registry.group<Audio::Command>().each([](auto entity, auto &command) {
            Audio::Play(command.soundName);
            Game::registry.destroy(entity);
        });
    }
}

// TODO this needs to be attached to an event system: everytime the player attacks this has to trigger updateAudio and updateAnimation and so on
//void updateAudio() {
//    playerView =Game::registry.view<Player,
//}
