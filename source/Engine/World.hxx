/*
 * Source/Engine/World.hxx
 *
 * This file is part of the Biq Invaders game source code.
 * Copyright 2023 Patrick Melo <patrick@patrickmelo.com.br>
 */

#ifndef BIQ_WORLD_HXX
#define BIQ_WORLD_HXX

#include "Engine/Types.hxx"

namespace Biq {

// World

class World {
    public:
        ~World() = default;

        // Object

        class Object {
            public:
                enum Type {
                    World = 0,
                    Player,
                    Enemy
                };

                Object(Type type) : type(type), image(NULL), speedMultiplier(1.0f) {}
                virtual ~Object() = default;

                uint        id;
                uint        layerIndex;
                Type        type;
                Image*      image;
                Vector2D    position;
                Vector2D    size;
                Vector2D    speed;
                float       speedMultiplier;
        };

        // Layer

        class Layer {
            public:
                Layer() : background(NULL) {}

                Image*                  background;
                std::map<uint, Object*> objects;
        };

		// Constants

		static constexpr charconst Tag = "World";

        // General

        static bool Initialize(const uint numberOfLayers);
        static void Finalize();
        static void Clear();
        static void Update(const float speedMultiplier);
        static void Render();

        // Layers

        static void SetLayerBackground(const uint layerIndex, Image* image);

        // Objects

        static void AddObject(const uint layerIndex, Object* object);
        static void RemoveObject(const Object* object);
        static bool CheckCollision(const Object* object1, const Object* object2);

    protected:
        World() = delete;

    private:
        static std::vector<Layer*> layers;
        static std::atomic<uint> objectCounter;
        static std::mutex mutex;
};

} // namespace Biq

#endif // BIQ_WORLD_HXX