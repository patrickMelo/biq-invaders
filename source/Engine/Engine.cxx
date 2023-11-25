/*
 * Source/Engine/Engine.cxx
 *
 * This file is part of the Biq Invaders game source code.
 * Copyright 2023 Patrick Melo <patrick@patrickmelo.com.br>
 */

#include "Engine/Engine.hxx"
#include "Engine/Renderer.hxx"
#include "Engine/World.hxx"
#include "Engine/Sound.hxx"

namespace Biq {

// String Table

namespace Txt {
	static const charconst InfoMessageFormat	= "\033[1;32m[%s] %s\033[0m\n";
	static const charconst WarningMessageFormat	= "\033[1;33m[%s] %s\033[0m\n";
	static const charconst ErrorMessageFormat	= "\033[1;31m[%s] %s\033[0m\n";
	static const charconst DebugMessageFormat	= "\033[1;35m[%s] %s\033[0m\n";
	static const charconst StubMessageFormat	= "\033[1;36m[Stub] %s in %s @ %ld\033[0m\n";

	static const charconst ProgramHeader		= "%s - Version %s (%s %s)";
    static const charconst DevelopmentVersion   = "--- DEVELOPMENT VERSION ---";

	static const charconst Running	= "Running";
	static const charconst Stopping	= "Stopping";
	static const charconst Stopped	= "Stopped";

    static const charconst StateAlreadyRegistered   = "There is already a state named \"%s\" registered";
    static const charconst StateNotFound            = "State \"%s\" not found";
    static const charconst StateRegistered          = "State \"%s\" registered";
    static const charconst StateChanged             = "Current state changed to \"%s\"";
}

// Static Members

GameInformation Engine::game;
std::map<string, State*> Engine::gameStates;

std::atomic<bool> Engine::isRunning(false);
State* Engine::currentState = NULL;

// General

bool Engine::Initialize(const GameInformation& gameInformation) {
	INFO(Txt::Empty);
	INFO(Txt::ProgramHeader, Engine::Name, Engine::VersionString, OSName, ArchName);
	INFO(Engine::CopyrightInfo);
	DEBUG(Txt::DevelopmentVersion);
	INFO(Txt::Empty);

    DEBUG(Txt::Initializing);

    if (!Renderer::Initialize(gameInformation)) {
        Finalize();
        return false;
    }

    if (!Sound::Initialize()) {
        Finalize();
        return false;
    }

    if (!World::Initialize(gameInformation.maxWorldLayers)) {
        Finalize();
        return false;
    }

    game = gameInformation;
    INFO(Txt::Initialized);
    return true;
}

void Engine::Finalize() {
    INFO(Txt::Finalizing);

    Stop();
    World::Finalize();
    Sound::Finalize();
    Renderer::Finalize();

    INFO(Txt::Finalized);
}

void Engine::Run(const charconst initialState) {
    if (isRunning) {
        return;
    }

    INFO(Txt::Running);

    isRunning = true;
    ChangeState(initialState);

    SDL_Event sdlEvent;

    int lastTick = GetTicks();
    int currentTick = lastTick;
    int lastFrameTime = 0;

    float frameTime = game.targetFPS / 1000.0f;

    while (isRunning) {
        currentTick = GetTicks();
        lastFrameTime = currentTick - lastTick;
        lastTick = currentTick;

        currentState->Step(lastFrameTime * frameTime);

        while (isRunning && (SDL_PollEvent(&sdlEvent) != 0)) {
            switch (sdlEvent.type) {
                case SDL_QUIT: {
                    Stop();
                    break;
                }

                case SDL_KEYDOWN: {
                    currentState->OnPress(SDLKeyToGameKey(sdlEvent.key.keysym.sym));
                    break;
                }

                case SDL_KEYUP: {
                    currentState->OnRelease(SDLKeyToGameKey(sdlEvent.key.keysym.sym));
                    break;
                }
            }
        }

        World::Render();
        Renderer::Update();
        std::this_thread::yield();
    }

    INFO(Txt::Stopping);

    currentState->Deactivate();

    INFO(Txt::Stopped);
}

void Engine::Stop() {
    isRunning = false;
}

// States

void Engine::RegisterState(const charconst stateName, const State& state) {
    auto existingState = gameStates.find(stateName);

    if (existingState != gameStates.end()) {
        WARNING(Txt::StateAlreadyRegistered, stateName);
        return;
    }

    gameStates[stateName] = (State*) &state;
    DEBUG(Txt::StateRegistered, stateName);
}

void Engine::ChangeState(const charconst stateName) {
    auto state = gameStates.find(stateName);

    if (state == gameStates.end()) {
        ERROR(Txt::StateNotFound, stateName);
        return;
    }

    if (isRunning) {
        if (currentState != NULL) {
            currentState->Deactivate();
        }

        state->second->Activate(game);
    }

    currentState = state->second;
    DEBUG(Txt::StateChanged, stateName);
}

// Log

#define LOG(messageFormat) \
	static char formattedMessage[8192]; \
	va_list functionArguments; \
	va_start(functionArguments, logMessage); \
	vsprintf(formattedMessage, logMessage, functionArguments); \
	va_end(functionArguments); \
    printf(messageFormat, logTag, formattedMessage);

void Engine::Info(const charconst logTag, const charconst logMessage, ...) {
    LOG(Txt::InfoMessageFormat);
}

void Engine::Warning(const charconst logTag, const charconst logMessage, ...) {
	LOG(Txt::WarningMessageFormat);
}

void Engine::Error(const charconst logTag, const charconst logMessage, ...) {
	LOG(Txt::ErrorMessageFormat);
}

void Engine::Debug(const charconst logTag, const charconst logMessage, ...) {
	#ifdef BIQ_DEBUG
		LOG(Txt::DebugMessageFormat);
	#endif
}

void Engine::Stub(const charconst functionName, const charconst fileName, const u64 lineNumber) {
	#ifdef BIQ_DEBUG
		printf(Txt::StubMessageFormat, functionName, fileName, lineNumber);
	#endif
}

// Utilities

uint Engine::GetTicks() {
    return SDL_GetTicks();
}

int Engine::RandomNumber(const int minValue, const int maxValue) {
    return (std::rand() % (maxValue - minValue + 1)) + minValue;
}

uint Engine::SDLKeyToGameKey(const SDL_Keycode sdlKey) {
    switch (sdlKey) {
        case SDLK_ESCAPE: return Input::KeyEscape;
        case SDLK_RETURN: return Input::KeyEnter;
        case SDLK_SPACE: return Input::KeySpacebar;
        case SDLK_a: return Input::KeyA;
        case SDLK_s: return Input::KeyS;
        case SDLK_d: return Input::KeyD;
        case SDLK_f: return Input::KeyF;
        case SDLK_UP: return Input::KeyUp;
        case SDLK_DOWN: return Input::KeyDown;
        case SDLK_LEFT: return Input::KeyLeft;
        case SDLK_RIGHT: return Input::KeyRight;
        default: return UINT(-1);
    }
}

} // namespace Biq
