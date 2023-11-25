/*
 * Source/Engine/Sound.hxx
 *
 * This file is part of the Biq Invaders game source code.
 * Copyright 2023 Patrick Melo <patrick@patrickmelo.com.br>
 */

#ifndef BIQ_SOUND_HXX
#define BIQ_SOUND_HXX

#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include "Engine/Types.hxx"

namespace Biq {

// Sound

class Sound {
    public:
        ~Sound() = default;

        // Constants

        static constexpr charconst Tag = "Sound";

        // General

        static bool Initialize();
        static void Finalize();

        // Samples

        static void* LoadSample(const string& filePath);
        static void UnloadSample(void* sample);
        static void PlaySample(void* sample);

        // Music

        static void* LoadMusic(const string& filePath);
        static void UnloadMusic(void* music);
        static void PlayMusic(void* music);
        static void StopMusic();

    protected:
        Sound() = delete;
};

} // namespace Biq

#endif // BIQ_SOUND_HXX