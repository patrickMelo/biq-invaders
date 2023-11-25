/*
 * Source/Game/Splash.hxx
 *
 * This file is part of the Biq Invaders game source code.
 * Copyright 2023 Patrick Melo <patrick@patrickmelo.com.br>
 */

#ifndef BIQ_GAME_SPLASH_HXX
#define BIQ_GAME_SPLASH_HXX

#include "Engine/Engine.hxx"

namespace Biq {
namespace Game {

class Splash : public State {
    public:
        Splash() = default;

        static constexpr charconst Tag  = "Splash";
        static constexpr charconst Name = "SPLASH";

        void Activate(const GameInformation& game);
        void Deactivate();
        void Step(const float speedMultiplier);
        void OnPress(const uint key);
        void OnRelease(const uint key);

    private:
        Image* splashImage;
};

} // namespace Game
} // namespace Biq

#endif // BIQ_GAME_SPLASH_HXX