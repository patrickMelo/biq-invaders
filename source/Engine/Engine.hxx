/*
 * Source/Engine/Engine.hxx
 *
 * This file is part of the Biq Invaders game source code.
 * Copyright 2023 Patrick Melo <patrick@patrickmelo.com.br>
 */

#ifndef BIQ_ENGINE_HXX
#define BIQ_ENGINE_HXX

#include "Engine/Types.hxx"
#include "SDL2/SDL.h"

// Macros

#define TEXT(untranslatedText) untranslatedText

#define INFO(message, ...)    Engine::Info(Tag, TEXT(message), ##__VA_ARGS__)
#define WARNING(message, ...) Engine::Warning(Tag, TEXT(message), ##__VA_ARGS__)
#define ERROR(message, ...)   Engine::Error(Tag, TEXT(message), ##__VA_ARGS__)
#define DEBUG(message, ...)   Engine::Debug(Tag, TEXT(message), ##__VA_ARGS__)
#define STUB()                Engine::Stub(__PRETTY_FUNCTION__, __FILE__, __LINE__)

namespace Biq {

// String Table

namespace Txt {
static constexpr charconst Empty        = "";
static constexpr charconst Initializing = "Initializing";
static constexpr charconst Finalizing   = "Finalizing";
static constexpr charconst Initialized  = "Initialized";
static constexpr charconst Finalized    = "Finalized";
}    // namespace Txt

namespace Input {
enum {
    KeyEscape = 0,
    KeyEnter,
    KeySpacebar,
    KeyA,
    KeyS,
    KeyD,
    KeyF,
    KeyUp,
    KeyDown,
    KeyLeft,
    KeyRight
};
}    // namespace Input

// State

class State {
    public:
        State()          = default;
        virtual ~State() = default;

        virtual void Activate(const GameInformation& game) = 0;
        virtual void Deactivate()                          = 0;
        virtual void Step(const float speedMultiplier)     = 0;
        virtual void OnPress(const uint key)               = 0;
        virtual void OnRelease(const uint key)             = 0;
};

// Engine

class Engine {
    public:
        ~Engine() = default;

        // Constants

        static constexpr charconst Tag           = "Biq";
        static constexpr charconst Name          = "Biq Engine";
        static constexpr uint      VersionNumber = 0x00000003;
        static constexpr charconst VersionString = "0.3";
        static constexpr charconst CopyrightInfo = "Copyright 2023 Patrick Melo <patrick@patrickmelo.com.br>";

        // General

        static bool Initialize(const GameInformation& gameInformation);
        static void Finalize();
        static void Run(const charconst initialStateName);
        static void Stop();

        // States

        static void RegisterState(const charconst stateName, const State& state);
        static void ChangeState(const charconst stateName);

        // Log

        static void Info(const charconst logTag, const charconst logMessage, ...);
        static void Warning(const charconst logTag, const charconst logMessage, ...);
        static void Error(const charconst logTag, const charconst logMessage, ...);
        static void Debug(const charconst logTag, const charconst logMessage, ...);
        static void Stub(const charconst functionName, const charconst fileName, const u64 lineNumber);

        // Utilities

        static uint GetTicks();
        static int  RandomNumber(const int minValue, const int maxValue);

    protected:
        Engine() = delete;

    private:
        static std::atomic<bool>        isRunning;
        static State*                   currentState;
        static std::map<string, State*> gameStates;
        static GameInformation          game;

        static uint SDLKeyToGameKey(const SDL_Keycode sdlKey);
};

}    // namespace Biq

#endif    // BIQ_ENGINE_HXX