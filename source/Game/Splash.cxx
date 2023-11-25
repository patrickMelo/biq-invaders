/*
 * Source/Game/Splash.cxx
 *
 * This file is part of the Biq Invaders game source code.
 * Copyright 2023 Patrick Melo <patrick@patrickmelo.com.br>
 */

#include "Game/Splash.hxx"

#include "Engine/Renderer.hxx"
#include "Engine/World.hxx"
#include "Game/InGame.hxx"

namespace Biq {
namespace Game {

void Splash::Activate(const GameInformation&) {
    splashImage = Renderer::LoadImage("assets/images/splash.jpg");
    World::Clear();
}

void Splash::Deactivate() {
    World::Clear();
    Renderer::UnloadImage(splashImage);
}

void Splash::Step(const float speedMultiplier) {
    World::SetLayerBackground(0, splashImage);
}

void Splash::OnPress(const uint key) {
    // Empty
}

void Splash::OnRelease(const uint key) {
    switch (key) {
        case Input::KeyEscape: {
            Engine::Stop();
            break;
        }

        case Input::KeyEnter: {
            Engine::ChangeState(InGame::Name);
            break;
        }
    }
}

}    // namespace Game
}    // namespace Biq