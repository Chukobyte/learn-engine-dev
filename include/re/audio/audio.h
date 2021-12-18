#pragma once

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
