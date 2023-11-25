/*
 * Source/Game/InGame.hxx
 *
 * This file is part of the Biq Invaders game source code.
 * Copyright 2023 Patrick Melo <patrick@patrickmelo.com.br>
 */

#ifndef BIQ_GAME_INGAME_HXX
#define BIQ_GAME_INGAME_HXX

#include "Engine/Engine.hxx"
#include "Engine/World.hxx"

namespace Biq {
namespace Game {

class ColoredObject : public World::Object {
    public:
        ColoredObject(World::Object::Type type) :
            World::Object(type) {
        }

        enum {
            Red = 0,
            Green,
            Blue,
            Black,
            MaxColors
        };

        uint color;
};

class PlayerObject : public ColoredObject {
    public:
        PlayerObject() :
            ColoredObject(World::Object::Player) {
        }

        int health;
        int score;
};

class EnemyObject : public ColoredObject {
    public:
        EnemyObject() :
            ColoredObject(World::Object::Enemy) {
        }

        int shotInterval;
        int nextShot;
        int yStop;
};

class CloudObject : public World::Object {
    public:
        CloudObject() :
            World::Object(World::Object::World) {
        }

        float distance;
};

class WorldObject : public World::Object {
    public:
        WorldObject() :
            World::Object(World::Object::World) {
        }
};

enum {
    BackgroundLayer = 0,
    LowCloudsLayer,
    ProjectileLayer,
    ShipLayer,
    HighCloudsLayer,
    HUDLayer,
    OverlayLayer,
    MaxLayers
};

class InGame : public State {
    public:
        static constexpr charconst Tag  = "InGame";
        static constexpr charconst Name = "INGAME";

        static constexpr int ShipWidth  = 72;
        static constexpr int ShipHeight = 72;

        static constexpr int ProjectileWidth  = 8;
        static constexpr int ProjectileHeight = 16;

        static constexpr int VerticalPadding   = 56;
        static constexpr int HorizontalPadding = 56;

        static constexpr float PlayerSpeed     = 20.0f;
        static constexpr float ProjectileSpeed = 25.0f;
        static constexpr float EnemySpeed      = 10.0f;

        static constexpr int EnemySpawnInterval  = 3280;
        static constexpr int EnemySpawnThreshold = 2;
        static constexpr int EnemyShootInterval  = 1200;

        static constexpr int NumberOfClouds = 32;
        static constexpr int CloudWidth     = 256;
        static constexpr int CloudHeight    = 256;
        static constexpr int CloudSpeed     = 20;

        static constexpr int LifebarHeight = 32;
        static constexpr int ScorePadding  = 8;

        void Activate(const GameInformation& game);
        void Deactivate();
        void Step(const float speedMultiplier);
        void OnPress(const uint key);
        void OnRelease(const uint key);

    private:
        std::atomic<bool> isGameOver;
        GameInformation   currentGame;

        Image* backgroundImage;
        Image* overlayImage;
        Image* cloudImages[4];
        Image* playerImages[ColoredObject::MaxColors];
        Image* enemyImages[ColoredObject::MaxColors];
        Image* projectileImages[ColoredObject::MaxColors];

        void LoadImages();
        void UnloadImages();

        void* shotSound;
        void* hitSound;
        void* clickSound;
        void* backgroundMusic;

        void LoadSounds();
        void UnloadSounds();

        float            currentSpeedMultiplier;
        int              currentTick;
        int              currentEnemySpawnInterval;
        std::atomic<int> enemySpawnCounter;

        WorldObject lifebar;
        WorldObject score;

        PlayerObject                player;
        std::vector<EnemyObject*>   enemies;
        std::vector<ColoredObject*> projectiles;
        std::vector<CloudObject*>   clouds;

        void InitializeObjects();
        void DeleteObjects();

        void StepClouds();
        void StepProjectiles();
        void StepEnemies();
        void StepPlayer();

        void Shoot(const ColoredObject* source, const int direction);
        void SpawnEnemy();
        void UpdateScore();
};

}    // namespace Game
}    // namespace Biq

#endif    // BIQ_GAME_INGAME_HXX