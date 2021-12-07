#ifndef AUDIO_HELPER_H
#define AUDIO_HELPER_H

#include "./re/audio/audio.h"
#include "./re/utils/helper.h"
#include "asset_manager.h"

class AudioHelper {
  public:
    static void PlayMusic(Music *music, bool loops = true) {
        int loopNumber = loops ? -1 : 0;
        Mix_PlayMusic(music->GetData(), loopNumber);
    }

    static void PlayMusic(const std::string &musicId, bool loops = true) {
        static AssetManager *assetManager = AssetManager::GetInstance();
        PlayMusic(assetManager->GetMusic(musicId), loops);
    }

    static void StopMusic() {
        Mix_HaltMusic();
    }

    static void PauseMusic() {
        Mix_PauseMusic();
    }

    static void PlaySound(SoundEffect *soundEffect) {
        Mix_PlayChannel(-1, soundEffect->GetData(), 0);
    }

    static void PlaySound(const std::string &soundId) {
        static AssetManager *assetManager = AssetManager::GetInstance();
        PlaySound(assetManager->GetSound(soundId));
    }

    static void SetMusicVolume(int volume) {
        Helper::Clamp<int>(volume, static_cast<int>(AudioVolume::MIN), static_cast<int>(AudioVolume::MAX));
        Mix_VolumeMusic(volume);
    }

    static void SetSoundVolume(int volume) {
        Helper::Clamp<int>(volume, static_cast<int>(AudioVolume::MIN), static_cast<int>(AudioVolume::MAX));
        static AssetManager *assetManager = AssetManager::GetInstance();
        for (auto const &pair : assetManager->GetAllSounds()) {
            SoundEffect *soundEffect = pair.second;
            Mix_VolumeChunk(soundEffect->GetData(), volume);
        }
    }

    static void SetAllVolume(int volume) {
        SetMusicVolume(volume);
        SetSoundVolume(volume);
    }
};

#endif //AUDIO_HELPER_H
