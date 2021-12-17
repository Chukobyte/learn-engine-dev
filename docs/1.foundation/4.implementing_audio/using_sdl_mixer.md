# Using SDL_mixer

## Audio

Creating `Music` and `SoundEffect` classes.

```c++
#ifndef AUDIO_H
#define AUDIO_H

#include <SDL2/SDL_mixer.h>

#include "../utils/logger.h"

enum class AudioVolume : int {
    MIN = 0,
    MAX = 128,
};

class Music {
  public:
    Music(const std::string &filePath) {
        mixMusic = Mix_LoadMUS(filePath.c_str());
        if (!mixMusic) {
            Logger::GetInstance()->Error("Error loading music at path '%s'\nSDL2 Mixer Error: %s",
                                         filePath.c_str(),
                                         Mix_GetError());
        }
    }

    ~Music() {
        if (mixMusic) {
            delete mixMusic;
        }
    }

    bool IsValid() const {
        return mixMusic != nullptr;
    }

    Mix_Music* GetData() {
        return mixMusic;
    }

  private:
    Mix_Music *mixMusic = nullptr;
};

class SoundEffect {
  public:
    SoundEffect(const std::string &filePath) {
        mixChunk = Mix_LoadWAV(filePath.c_str());
        if (!mixChunk) {
            Logger::GetInstance()->Error("Error loading sound at path '%s'\nSDL2 Mixer Error: %s",
                                         filePath.c_str(),
                                         Mix_GetError());
        }
    }

    ~SoundEffect() {
        if (mixChunk) {
            delete mixChunk;
        }
    }
    bool IsValid() const {
        return mixChunk != nullptr;
    }

    Mix_Chunk* GetData() {
        return mixChunk;
    }

  private:
    Mix_Chunk *mixChunk = nullptr;
};

#endif //AUDIO_H
```

## Asset Manager

Will need to add functions for `Music` and `SoundEffect` in the `AssetManager` class.

```c++
#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <unordered_map>
#include <string>

#include "./re/audio/audio.h"
#include "./re/utils/logger.h"
#include "./re/rendering/texture.h"
#include "./re/rendering/font.h"
#include "./re/rendering/render_context.h"


class AssetManager {
  public:
    static AssetManager* GetInstance();
    // Texture
    void LoadTexture(const std::string &id, const std::string &filePath);
    Texture* GetTexture(const std::string &id);
    bool HasTexture(const std::string &id) const;
    // Font
    void LoadFont(const std::string &fontId, const std::string &fontPath, int size);
    Font* GetFont(const std::string &fontId);
    bool HasFont(const std::string &fontId) const;
    // Music
    void LoadMusic(const std::string &musicId, const std::string &musicPath);
    Music* GetMusic(const std::string &musicId);
    bool HasMusic(const std::string &musicId) const;
    // Sound
    void LoadSound(const std::string &soundId, const std::string &soundPath);
    SoundEffect* GetSound(const std::string &soundId);
    bool HasSound(const std::string &soundId) const;
    std::unordered_map<std::string, SoundEffect*> GetAllSounds();

  private:
    std::unordered_map<std::string, Texture*> textures;
    std::unordered_map<std::string, Font*> fonts;
    std::unordered_map<std::string, Music*> music;
    std::unordered_map<std::string, SoundEffect*> soundEffects;
    RenderContext *renderContext = nullptr;
    Logger *logger = nullptr;

    AssetManager();
};

#endif //ASSET_MANAGER_H
```

```c++
void AssetManager::LoadMusic(const std::string &musicId, const std::string &musicPath) {
    if (HasMusic(musicId)) {
        logger->Warn("Already has music, not loading!");
        return;
    }
    Music *newMusic = new Music(musicPath);
    assert(newMusic->IsValid() && "Failed to load music!");
    music.emplace(musicId, newMusic);
}

Music *AssetManager::GetMusic(const std::string &musicId) {
    assert(HasMusic(musicId) && "Failed to get music!");
    return music[musicId];
}

bool AssetManager::HasMusic(const std::string &musicId) const {
    return music.count(musicId) > 0;
}

void AssetManager::LoadSound(const std::string &soundId, const std::string &soundPath) {
    if (HasSound(soundId)) {
        logger->Warn("Already have sound, not loading!");
        return;
    }
    SoundEffect *soundEffect = new SoundEffect(soundPath);
    assert(soundEffect->IsValid() && "Failed to load sound effect!");
    soundEffects.emplace(soundId, soundEffect);
}

SoundEffect *AssetManager::GetSound(const std::string &soundId) {
    assert(HasSound(soundId) && "Failed to get sound effect!");
    return soundEffects[soundId];
}

bool AssetManager::HasSound(const std::string &soundId) const {
    return soundEffects.count(soundId) > 0;
}

std::unordered_map<std::string, SoundEffect *> AssetManager::GetAllSounds() {
    return soundEffects;
}
```

I haven't included the entire `asset_manager.cpp` file to reduce lines.

## Audio Helper

The `AudioHelper` class contains static functions used to interact with music and sound effects.

```c++
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
```

## Using Audio

We'll need to make a few tweaks to the `GameEngine` class.

```c++
bool GameEngine::InitializeAudio() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        logger->Error("SDL_mixer could not be initialized!");
        return false;
    }

    // Temp load assets
    assetManager->LoadMusic("test_music", "assets/audio/music/test_music.wav");
    assetManager->LoadSound("test_sound", "assets/audio/sound/test_sound_effect.wav");
    return true;
}
```

```c++
bool GameEngine::Initialize() {
    logger->Debug("Initializing...");
    if (!InitializeSDL()) {
        logger->Error("Failed to initialize SDL!");
        return false;
    }
    if (!InitializeAudio()) {
        logger->Error("Failed to initialize audio!");
        return false;
    }
    if (!InitializeRendering()) {
        logger->Error("Failed to initialize rendering!");
        return false;
    }
    logger->Info("%s Engine v%s", engineContext->GetEngineName(), engineContext->GetEngineVersion());
    engineContext->SetRunning(true);

    // Temp play music
    AudioHelper::PlayMusic("test_music");
    return true;
}
```

In the initialize function we are now calling `InitializeAudio`.  There is also a call to `AudioHelper::PlayMusic` to play music when the engine initializes.

```c++
void GameEngine::Update() {
    // Sleep until FRAME_TARGET_TIME has elapsed since last frame
    const unsigned int MILLISECONDS_PER_TICK = 1000;
    const unsigned int TARGET_FPS = 60;
    static Uint32 lastFrameTime = 0;
    const unsigned int FRAME_TARGET_TIME = MILLISECONDS_PER_TICK / TARGET_FPS;
    unsigned int timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - lastFrameTime);
    if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME) {
        SDL_Delay(timeToWait);
    }

    fpsCounter->Update();

    // Temporary timer to play sound after 3 seconds
    static Timer *timer = nullptr;
    if (!timer) {
        timer = new Timer(3.0f);
        timer->Start();
    }
    static bool hasPlayedSound = false;
    if (timer->HasReachedTimeOut() && !hasPlayedSound) {
        AudioHelper::PlaySound("test_sound");
        hasPlayedSound = true;
    }

    lastFrameTime = SDL_GetTicks();
}
```

In the `Update` function, a timer is created to play a sound effect after 3 seconds.  If you run the code, it should play song and a brief sound effect after 3 seconds once the engine is running.  The source code for this section can be found [here](https://github.com/Chukobyte/learn-engine-dev/tree/main/src/1.foundation/4.implementing_audio/4.0.sdl_mixer).  Now that we have music and sound effects implemented it's time to tackle handling input next!
