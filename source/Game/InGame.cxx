/*
 * Source/Game/InGame.cxx
 *
 * This file is part of the Biq Invaders game source code.
 * Copyright 2023 Patrick Melo <patrick@patrickmelo.com.br>
 */

#include "Game/InGame.hxx"

#include "Engine/Renderer.hxx"
#include "Engine/Sound.hxx"
#include "Game/Splash.hxx"

namespace Biq {
namespace Game {

void InGame::Activate(const GameInformation& game) {
    World::Clear();

    currentGame = game;

    LoadImages();
    LoadSounds();
    InitializeObjects();
    UpdateScore();
}

void InGame::Deactivate() {
    World::Clear();

    DeleteObjects();
    UnloadImages();
    UnloadSounds();
}

void InGame::InitializeObjects() {
    World::SetLayerBackground(Game::BackgroundLayer, backgroundImage);
    World::SetLayerBackground(Game::OverlayLayer, overlayImage);

    // Player

    player.health     = 100;
    player.score      = 0;
    player.color      = ColoredObject::Red;
    player.image      = playerImages[player.color];
    player.speed      = {0.0f, 0.0f};
    player.size.x     = InGame::ShipWidth;
    player.size.y     = InGame::ShipHeight;
    player.position.x = (currentGame.targetWidth - InGame::ShipWidth) / 2;
    player.position.y = currentGame.targetHeight - (InGame::ShipHeight + InGame::VerticalPadding);

    World::AddObject(Game::ShipLayer, &player);

    // Lifebar

    if (lifebar.image != NULL) {
        lifebar.position.x = 0;
        lifebar.position.y = currentGame.targetHeight - lifebar.image->height;
        lifebar.size.x     = currentGame.targetWidth;
        lifebar.size.y     = InGame::LifebarHeight;
        lifebar.speed.x    = 0.0f;
        lifebar.speed.y    = 0.0f;

        World::AddObject(Game::HUDLayer, &lifebar);
    }

    // Score

    score.speed.x = 0.0f;
    score.speed.y = 0.0f;

    World::AddObject(Game::HUDLayer, &score);

    // Clouds

    for (auto cloudIndex = 0; cloudIndex < InGame::NumberOfClouds; cloudIndex++) {
        auto cloudObject = new CloudObject();

        cloudObject->distance   = Engine::RandomNumber(5, 20) / 10.0f;
        cloudObject->image      = cloudImages[Engine::RandomNumber(0, 3)];
        cloudObject->position.x = Engine::RandomNumber(-InGame::HorizontalPadding, currentGame.targetWidth - InGame::CloudWidth + InGame::HorizontalPadding);
        cloudObject->position.y = -Engine::RandomNumber(InGame::CloudHeight, InGame::CloudHeight * 2);
        cloudObject->size.x     = static_cast<float>(InGame::CloudHeight) / cloudObject->distance;
        cloudObject->size.y     = static_cast<float>(InGame::CloudWidth) / cloudObject->distance;
        cloudObject->speed.x    = 0.0f;
        cloudObject->speed.y    = InGame::CloudSpeed / cloudObject->distance;

        clouds.push_back(cloudObject);

        World::AddObject(cloudObject->distance > 1.0f ? Game::LowCloudsLayer : Game::HighCloudsLayer, cloudObject);
    }

    // General

    currentEnemySpawnInterval = InGame::EnemySpawnInterval;
    enemySpawnCounter         = 0;
    isGameOver                = false;
}

void InGame::DeleteObjects() {
    for (auto object : clouds) {
        delete object;
    }

    clouds.clear();

    for (auto object : enemies) {
        delete object;
    }

    enemies.clear();

    for (auto object : projectiles) {
        delete object;
    }

    projectiles.clear();
}

void InGame::LoadImages() {
    score.image   = NULL;
    lifebar.image = Renderer::LoadImage("assets/images/lifebar.png");

    backgroundImage                        = Renderer::LoadImage("assets/images/background.jpg");
    overlayImage                           = Renderer::LoadImage("assets/images/overlay.png");
    cloudImages[0]                         = Renderer::LoadImage("assets/images/cloud1.png");
    cloudImages[1]                         = Renderer::LoadImage("assets/images/cloud2.png");
    cloudImages[2]                         = Renderer::LoadImage("assets/images/cloud3.png");
    cloudImages[3]                         = Renderer::LoadImage("assets/images/cloud4.png");
    playerImages[ColoredObject::Red]       = Renderer::LoadImage("assets/images/player_red.png");
    playerImages[ColoredObject::Green]     = Renderer::LoadImage("assets/images/player_green.png");
    playerImages[ColoredObject::Blue]      = Renderer::LoadImage("assets/images/player_blue.png");
    playerImages[ColoredObject::Black]     = Renderer::LoadImage("assets/images/player_black.png");
    enemyImages[ColoredObject::Red]        = Renderer::LoadImage("assets/images/enemy_red.png");
    enemyImages[ColoredObject::Green]      = Renderer::LoadImage("assets/images/enemy_green.png");
    enemyImages[ColoredObject::Blue]       = Renderer::LoadImage("assets/images/enemy_blue.png");
    enemyImages[ColoredObject::Black]      = Renderer::LoadImage("assets/images/enemy_black.png");
    projectileImages[ColoredObject::Red]   = Renderer::LoadImage("assets/images/projectile_red.png");
    projectileImages[ColoredObject::Green] = Renderer::LoadImage("assets/images/projectile_green.png");
    projectileImages[ColoredObject::Blue]  = Renderer::LoadImage("assets/images/projectile_blue.png");
    projectileImages[ColoredObject::Black] = Renderer::LoadImage("assets/images/projectile_black.png");
}

void InGame::UnloadImages() {
    if (score.image != NULL) {
        Renderer::UnloadImage(score.image);
    }

    Renderer::UnloadImage(backgroundImage);
    Renderer::UnloadImage(overlayImage);
    Renderer::UnloadImage(playerImages[ColoredObject::Red]);
    Renderer::UnloadImage(playerImages[ColoredObject::Green]);
    Renderer::UnloadImage(playerImages[ColoredObject::Blue]);
    Renderer::UnloadImage(playerImages[ColoredObject::Black]);
    Renderer::UnloadImage(cloudImages[0]);
    Renderer::UnloadImage(cloudImages[1]);
    Renderer::UnloadImage(cloudImages[2]);
    Renderer::UnloadImage(cloudImages[3]);
    Renderer::UnloadImage(enemyImages[ColoredObject::Red]);
    Renderer::UnloadImage(enemyImages[ColoredObject::Green]);
    Renderer::UnloadImage(enemyImages[ColoredObject::Blue]);
    Renderer::UnloadImage(enemyImages[ColoredObject::Black]);
    Renderer::UnloadImage(projectileImages[ColoredObject::Red]);
    Renderer::UnloadImage(projectileImages[ColoredObject::Green]);
    Renderer::UnloadImage(projectileImages[ColoredObject::Blue]);
    Renderer::UnloadImage(projectileImages[ColoredObject::Black]);
}

void InGame::LoadSounds() {
    shotSound       = Sound::LoadSample("assets/sounds/shot.flac");
    hitSound        = Sound::LoadSample("assets/sounds/hit.flac");
    clickSound      = Sound::LoadSample("assets/sounds/click.flac");
    backgroundMusic = Sound::LoadMusic("assets/sounds/background.flac");

    Sound::PlayMusic(backgroundMusic);
}

void InGame::UnloadSounds() {
    Sound::StopMusic();

    Sound::UnloadSample(shotSound);
    Sound::UnloadSample(hitSound);
    Sound::UnloadSample(clickSound);
    Sound::UnloadMusic(backgroundMusic);
}

void InGame::StepClouds() {
    for (auto cloud : clouds) {
        if (cloud->position.y > currentGame.targetHeight) {
            cloud->position.x = Engine::RandomNumber(-InGame::HorizontalPadding, currentGame.targetWidth - InGame::CloudWidth + InGame::HorizontalPadding);
            cloud->position.y = -Engine::RandomNumber(InGame::CloudHeight, InGame::CloudHeight * 2);
        }
    }
}

void InGame::StepProjectiles() {
    static bool projectileHit = false;

    auto projectileIterator = projectiles.begin();
    while (projectileIterator != projectiles.end()) {
        auto projectile = *projectileIterator;

        projectileHit = false;

        // Check player hit.
        if (projectile->type == World::Object::Enemy) {
            if ((projectileHit = World::CheckCollision(&player, projectile))) {
                player.health -= (projectile->color + 1) * 5;

                lifebar.size.x = (player.health * currentGame.targetWidth) / 100.0f;

                if (player.health <= 0) {
                    isGameOver = true;
                    Sound::PlaySample(hitSound);
                    UpdateScore();
                    World::Update(currentSpeedMultiplier);
                    return;
                }
            }
        } else {    // Check enemy hit.
            for (auto enemyIterator = enemies.begin(); enemyIterator != enemies.end(); enemyIterator++) {
                auto enemy = *enemyIterator;

                if (enemy->color != projectile->color) {
                    continue;
                }

                if ((projectileHit = World::CheckCollision(enemy, projectile))) {
                    World::RemoveObject(enemy);
                    enemies.erase(enemyIterator);
                    delete enemy;

                    player.score += (projectile->color + 1) * 5;
                    UpdateScore();
                    break;
                }
            }
        }

        if (projectileHit) {
            Sound::PlaySample(hitSound);
        }

        if (projectileHit || (projectile->position.y <= -InGame::ProjectileHeight) || (projectile->position.y >= currentGame.targetHeight)) {
            World::RemoveObject(projectile);
            projectiles.erase(projectileIterator);
            delete projectile;
            continue;
        }

        projectileIterator++;
    }
}

void InGame::StepEnemies() {
    static int nextEnemySpawn = currentTick + currentEnemySpawnInterval;

    if (currentTick >= nextEnemySpawn) {
        nextEnemySpawn = currentTick + currentEnemySpawnInterval;
        SpawnEnemy();
    }

    auto enemyIterator = enemies.begin();
    while (enemyIterator != enemies.end()) {
        auto enemy = *enemyIterator;

        if (enemy->position.y > enemy->yStop) {
            enemy->position.y = enemy->yStop;
            enemy->speed.y    = 0.0f;
            enemy->speed.x    = Engine::RandomNumber(0, 1) == 1 ? InGame::EnemySpeed : -InGame::EnemySpeed;
        }

        if (enemy->position.x >= currentGame.targetWidth - (InGame::ShipWidth + InGame::HorizontalPadding)) {
            enemy->speed.x = -InGame::EnemySpeed;
        }

        if (enemy->position.x <= InGame::HorizontalPadding) {
            enemy->speed.x = InGame::EnemySpeed;
        }

        if (currentTick > enemy->nextShot) {
            enemy->nextShot = currentTick + enemy->shotInterval;
            Shoot(enemy, 1);
        }

        enemyIterator++;
    }
}

void InGame::Step(const float speedMultiplier) {
    if (isGameOver) {
        return;
    }

    currentSpeedMultiplier = speedMultiplier;
    currentTick            = Engine::GetTicks();

    StepClouds();
    StepProjectiles();
    StepEnemies();
    StepPlayer();

    World::Update(speedMultiplier);
}

void InGame::StepPlayer() {
    if (player.position.x >= currentGame.targetWidth - (InGame::ShipWidth + InGame::HorizontalPadding)) {
        player.position.x = currentGame.targetWidth - (InGame::ShipWidth + InGame::HorizontalPadding);
    }

    if (player.position.x <= InGame::HorizontalPadding) {
        player.position.x = InGame::HorizontalPadding;
    }

    if (player.position.y >= currentGame.targetHeight - (InGame::ShipHeight + InGame::VerticalPadding)) {
        player.position.y = currentGame.targetHeight - (InGame::ShipHeight + InGame::VerticalPadding);
    }

    if (player.position.y <= InGame::VerticalPadding) {
        player.position.y = InGame::VerticalPadding;
    }
}

void InGame::Shoot(const ColoredObject* source, const int direction) {
    if (isGameOver) {
        return;
    }

    auto projectile = new ColoredObject(source->type);

    projectile->type       = source->type;
    projectile->color      = source->color;
    projectile->position.y = source->position.y + ((InGame::ShipHeight - InGame::ProjectileHeight) / 2.0f);
    projectile->position.x = source->position.x + ((InGame::ShipWidth - InGame::ProjectileWidth) / 2.0f);
    projectile->size.x     = InGame::ProjectileWidth;
    projectile->size.y     = InGame::ProjectileHeight;
    projectile->speed.x    = 0.0f;
    projectile->speed.y    = InGame::ProjectileSpeed * direction;
    projectile->image      = projectileImages[projectile->color];

    projectiles.push_back(projectile);
    World::AddObject(Game::ProjectileLayer, projectile);

    Sound::PlaySample(shotSound);
}

void InGame::SpawnEnemy() {
    auto enemy = new EnemyObject();

    enemy->color           = Engine::RandomNumber(ColoredObject::Red, ColoredObject::Black);
    enemy->speed.y         = InGame::EnemySpeed;
    enemy->speed.x         = 0.0f;
    enemy->size.x          = InGame::ShipWidth;
    enemy->size.y          = InGame::ShipHeight;
    enemy->position.x      = Engine::RandomNumber(InGame::HorizontalPadding, currentGame.targetWidth - InGame::ShipWidth - InGame::HorizontalPadding);
    enemy->position.y      = -InGame::ShipHeight;
    enemy->speedMultiplier = 1.0f + (F32(Engine::RandomNumber(0, 100)) / 100.f);
    enemy->shotInterval    = Engine::RandomNumber(InGame::EnemyShootInterval * 0.9f, InGame::EnemyShootInterval * 1.5f);
    enemy->nextShot        = currentTick + enemy->shotInterval;
    enemy->yStop           = InGame::VerticalPadding * (Engine::RandomNumber(10, 20) / 10.0f);
    enemy->image           = enemyImages[enemy->color];

    enemySpawnCounter++;

    if (enemySpawnCounter > InGame::EnemySpawnThreshold) {
        enemySpawnCounter = 0;
        currentEnemySpawnInterval *= 0.9;
    }

    enemies.push_back(enemy);
    World::AddObject(ShipLayer, enemy);
}

void InGame::OnPress(const uint key) {
    if (isGameOver) {
        if (key == Input::KeyEnter) {
            World::Clear();

            DeleteObjects();
            InitializeObjects();
            UpdateScore();
        }

        return;
    }

    switch (key) {
        case Input::KeyLeft: {
            player.speed.x = -InGame::PlayerSpeed;
            break;
        }

        case Input::KeyRight: {
            player.speed.x = InGame::PlayerSpeed;
            break;
        }

        case Input::KeyUp: {
            player.speed.y = -InGame::PlayerSpeed;
            break;
        }

        case Input::KeyDown: {
            player.speed.y = InGame::PlayerSpeed;
            break;
        }

        case Input::KeySpacebar: {
            Shoot(&player, -1);
            break;
        }

        case Input::KeyA:
        case Input::KeyS:
        case Input::KeyD:
        case Input::KeyF: {
            player.color = key - Input::KeyA;
            player.image = playerImages[player.color];

            Sound::PlaySample(clickSound);
            break;
        }
    }
}

void InGame::OnRelease(const uint key) {
    switch (key) {
        case Input::KeyEscape: {
            Engine::ChangeState(Splash::Name);
            break;
        }

        case Input::KeyUp: {
            if (player.speed.y == -InGame::PlayerSpeed) {
                player.speed.y = 0.0f;
            }

            break;
        }
        case Input::KeyDown: {
            if (player.speed.y == InGame::PlayerSpeed) {
                player.speed.y = 0.0f;
            }

            break;
        }

        case Input::KeyLeft: {
            if (player.speed.x == -InGame::PlayerSpeed) {
                player.speed.x = 0.0f;
            }

            break;
        }
        case Input::KeyRight: {
            if (player.speed.x == InGame::PlayerSpeed) {
                player.speed.x = 0.0f;
            }

            break;
        }
    }
}

void InGame::UpdateScore() {
    if (score.image != NULL) {
        Renderer::UnloadImage(score.image);
    }

    if (isGameOver) {
        score.image      = Renderer::TextImage("GAME OVER | YOU SCORED " + std::to_string(player.score) + " | PRESS <ENTER> TO RESTART");
        score.position.y = (currentGame.targetHeight - score.image->height) / 2.0f;
    } else {
        score.image      = Renderer::TextImage("SCORE: " + std::to_string(player.score));
        score.position.y = InGame::ScorePadding;
    }

    score.position.x = (currentGame.targetWidth - score.image->width) / 2.0f;
    score.size.x     = score.image->width;
    score.size.y     = score.image->height;
}

}    // namespace Game
}    // namespace Biq