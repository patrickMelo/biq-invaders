/*
 * Source/Engine/World.cxx
 *
 * This file is part of the Biq Invaders game source code.
 * Copyright 2023 Patrick Melo <patrick@patrickmelo.com.br>
 */

#include "Engine/Engine.hxx"
#include "Engine/World.hxx"
#include "Engine/Renderer.hxx"

namespace Biq {

// String Table

namespace Txt {
    static const charconst InitializingWorld    = "Initializing world with %d layers";
    static const charconst Cleared              = "Cleared";
}

// Static Members

std::vector<World::Layer*> World::layers;
std::atomic<uint> World::objectCounter;
std::mutex World::mutex;

// General

bool World::Initialize(const uint numberOfLayers) {
    DEBUG(Txt::InitializingWorld, numberOfLayers);

    for (auto layerIndex = 0; layerIndex < numberOfLayers; layerIndex++) {
        layers.push_back(new Layer());
    }

    DEBUG(Txt::Initialized);
    return true;
}

void World::Finalize() {
    DEBUG(Txt::Finalizing);

    for (auto layer : layers) {
        layer->objects.clear();
        delete layer;
    }

    layers.clear();
    objectCounter = 0;

    DEBUG(Txt::Finalized);
}

void World::Clear() {
    mutex.lock();

    for (auto layer : layers) {
        layer->background = NULL;
        layer->objects.clear();
    }

    objectCounter = 0;

    mutex.unlock();
    DEBUG(Txt::Cleared);
}

void World::Update(const float speedMultiplier) {
    for (auto layer : layers) {
        for (auto objectIterator : layer->objects) {
            auto object = objectIterator.second;
            object->position.x += object->speed.x * object->speedMultiplier * speedMultiplier;
            object->position.y += object->speed.y * object->speedMultiplier * speedMultiplier;
        }
    }
}

void World::Render() {
    for (auto layer : layers) {
        if (layer->background != NULL) {
            Renderer::Splash(layer->background);
        }

        for (auto object : layer->objects) {
            // TODO: skip objects outside viewport.
            Renderer::Draw(object.second->image, object.second->position, object.second->size);
        }
    }
}

// Layers

void World::SetLayerBackground(const uint layerIndex, Image* image) {
    if (layerIndex >= layers.size()) {
        return;
    }

    mutex.lock(); // FIXME: there must be a better way of doing this.
    layers[layerIndex]->background = image;
    mutex.unlock();
}

// Objects

void World::AddObject(const uint layerIndex, Object* object) {
    if (layerIndex >= layers.size()) {
        return;
    }

    mutex.lock(); // FIXME: there must be a better way of doing this.

    object->id = objectCounter++;
    object->layerIndex = layerIndex;
    layers[layerIndex]->objects[object->id] = object;

    mutex.unlock();
}

void World::RemoveObject(const Object* object) {
    if (object == NULL) {
        return;
    }

    mutex.lock(); // FIXME: there must be a better way of doing this.
    layers[object->layerIndex]->objects.erase(object->id);
    mutex.unlock();
}

bool World::CheckCollision(const Object* object1, const Object* object2) {
    return
        (object1->position.x + object1->size.x > object2->position.x) && (object1->position.x < object2->position.x + object2->size.x) &&
        (object1->position.y + object1->size.y > object2->position.y) && (object1->position.y < object2->position.y + object2->size.y);
}

} // namespace Biq
