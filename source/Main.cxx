#include "Engine/Engine.hxx"
#include "Game/InGame.hxx"
#include "Game/Splash.hxx"

int main(int numberOfArguments, char** argumentsValues) {
    static constexpr char const* gameName = "Biq Invaders";

    if (!Biq::Engine::Initialize({ const_cast<char*>(gameName), 1280, 720, 30, Biq::Game::MaxLayers })) {
        return 1;
    }

    Biq::Game::Splash splashState;
    Biq::Game::InGame inGameState;

    Biq::Engine::RegisterState(Biq::Game::Splash::Name, splashState);
    Biq::Engine::RegisterState(Biq::Game::InGame::Name, inGameState);

    Biq::Engine::Run(Biq::Game::Splash::Name);
    Biq::Engine::Finalize();
    return 0;
}