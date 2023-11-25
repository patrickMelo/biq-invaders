/*
 * Source/Engine/Sound.cxx
 *
 * This file is part of the Biq Invaders game source code.
 * Copyright 2023 Patrick Melo <patrick@patrickmelo.com.br>
 */

#include "Engine/Engine.hxx"
#include "Engine/Sound.hxx"

namespace Biq {

// String Table

namespace Txt {
    static const charconst CouldNotInitializeSound      = "Could not initialize the sound: %s";
    static const charconst CouldNotInitializeSDLMixer   = "Could not initialize the SDL_mixer library: %s";
    static const charconst CouldNotLoadSample           = "Could not load audio sample from \"%s\": %s";
    static const charconst CouldNotLoadMusic            = "Could not load music from \"%s\": %s";

    static const charconst SampleLoaded     = "Sample loaded from \"%s\"";
    static const charconst SampleUnloaded   = "Sample unloaded";
    static const charconst MusicLoaded      = "Music loaded from \"%s\"";
    static const charconst MusicUnloaded    = "Music unloaded";
}

// General

bool Sound::Initialize() {
    DEBUG(Txt::Initializing);

    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        ERROR(Txt::CouldNotInitializeSound, SDL_GetError());
        return false;
    }

    if (Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
        ERROR(Txt::CouldNotInitializeSDLMixer, Mix_GetError());
        return false;
    }

    DEBUG(Txt::Initialized);
    return true;
}

void Sound::Finalize() {
    DEBUG(Txt::Finalizing);
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    DEBUG(Txt::Finalized);
}

// Samples

void* Sound::LoadSample(const std::string& filePath) {
    auto sample = Mix_LoadWAV(filePath.c_str());

    if (sample == NULL) {
        WARNING(Txt::CouldNotLoadSample, filePath.c_str(), Mix_GetError());
    }

    DEBUG(Txt::SampleLoaded, filePath.c_str());
    return sample;
}

void Sound::UnloadSample(void* sample) {
    if (sample == NULL) {
        return;
    }

    Mix_FreeChunk((Mix_Chunk*) sample);
    DEBUG(Txt::SampleUnloaded);
}

void Sound::PlaySample(void* sample) {
    if (sample == NULL) {
        return;
    }

    Mix_PlayChannel(-1, (Mix_Chunk*) sample, 0);
}

// Music

void* Sound::LoadMusic(const std::string& filePath) {
    auto music = Mix_LoadMUS(filePath.c_str());

    if (music == NULL) {
        WARNING(Txt::CouldNotLoadMusic, filePath.c_str(), Mix_GetError());
    }

    DEBUG(Txt::MusicLoaded, filePath.c_str());
    return music;
}

void Sound::UnloadMusic(void* music) {
    if (music == NULL) {
        return;
    }

    Mix_FreeMusic((Mix_Music*) music);
    DEBUG(Txt::MusicUnloaded);
}

void Sound::PlayMusic(void* music) {
    if (music == NULL) {
        return;
    }

    Mix_PlayMusic((Mix_Music*) music, -1);
}

void Sound::StopMusic() {
    Mix_PauseMusic();
}

} // namespace Biq