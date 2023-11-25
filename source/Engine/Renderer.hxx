/*
 * Source/Engine/Renderer.hxx
 *
 * This file is part of the Biq Invaders game source code.
 * Copyright 2023 Patrick Melo <patrick@patrickmelo.com.br>
 */

#ifndef BIQ_RENDERER_HXX
#define BIQ_RENDERER_HXX

#include "Engine/Types.hxx"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

namespace Biq {

// Renderer

class Renderer {
    public:
        ~Renderer() = default;

        // Constants

        static constexpr charconst Tag             = "Renderer";
        static constexpr charconst DefaultFontPath = "assets/font.ttf";
        static constexpr int       TextSize        = 36;

        // General

        static bool Initialize(const GameInformation& gameInformation);
        static void Finalize();
        static void Update();

        // Drawing

        static void Splash(const Image* image);
        static void Draw(const Image* image, const Vector2D& position, const Vector2D& size);

        // Images

        static Image* LoadImage(const string& filePath);
        static void   UnloadImage(const Image* image);

        // Text

        static Image* TextImage(const string& text);

    protected:
        Renderer() = delete;

    private:
        static SDL_Rect      windowRect;
        static SDL_Window*   sdlWindow;
        static SDL_Renderer* sdlRenderer;
        static TTF_Font*     textFont;

        static Image* ImageFromSurface(SDL_Surface* surface);
};

}    // namespace Biq

#endif    // BIQ_RENDERER_HXX