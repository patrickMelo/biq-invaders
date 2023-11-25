/*
 * Source/Engine/Renderer.cxx
 *
 * This file is part of the Biq Invaders game source code.
 * Copyright 2023 Patrick Melo <patrick@patrickmelo.com.br>
 */

#include "Engine/Renderer.hxx"

#include "Engine/Engine.hxx"

namespace Biq {

// String Table

namespace Txt {
static const charconst CouldNotInitializeRenderer    = "Could not initialize the renderer: %s";
static const charconst CouldNotCreateRendererWindow  = "Could not create the renderer window: %s";
static const charconst CouldNotCreateRendererContext = "Could not create the renderer context: %s";
static const charconst CouldNotInitializeSDLImage    = "Could not initialize the SDL_image library: %s";
static const charconst CouldNotInitializeSDLTTF      = "Could not initialize the SDL_ttf library: %s";
static const charconst CouldNotLoadDefaultFont       = "Could not load the default font: %s";
static const charconst CouldNotLoadImage             = "Could not load the image from \"%s\": %s";
static const charconst CouldNotCreateImageTexture    = "Could not create the image texture: %s";
static const charconst CouldNotCreateTextTexture     = "Could not create the text texture: %s";

static const charconst CreatingRendererWindow  = "Creating renderer window";
static const charconst CreatingRendererContext = "Creating renderer context";
static const charconst InitializingSDLImage    = "Initializing SDL_image";
static const charconst InitializingSDLTTF      = "Initializing SDL_ttf";
static const charconst LoadingDefaultFont      = "Loading default font from \"%s\"";

static const charconst UnloadingDefaultFont      = "Unloading default font";
static const charconst DestroyingRendererContext = "Destroying renderer context";
static const charconst DestroyingRendererWindow  = "Destroying renderer window";

static const charconst ImageLoaded = "Image loaded from \"%s\"";
}    // namespace Txt

// Static Members

SDL_Rect Renderer::windowRect;

SDL_Window*   Renderer::sdlWindow   = NULL;
SDL_Renderer* Renderer::sdlRenderer = NULL;
TTF_Font*     Renderer::textFont    = NULL;

// General

bool Renderer::Initialize(const GameInformation& gameInformation) {
    DEBUG(Txt::Initializing);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        ERROR(Txt::CouldNotInitializeRenderer, SDL_GetError());
        return false;
    }

    DEBUG(Txt::CreatingRendererWindow);

    sdlWindow = SDL_CreateWindow(gameInformation.name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, gameInformation.targetWidth, gameInformation.targetHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_UTILITY);

    if (sdlWindow == NULL) {
        ERROR(Txt::CouldNotCreateRendererWindow, SDL_GetError());
        return false;
    }

    windowRect.x = 0;
    windowRect.y = 0;
    windowRect.w = gameInformation.targetWidth;
    windowRect.h = gameInformation.targetHeight;

    DEBUG(Txt::CreatingRendererContext);

    sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED);

    if (sdlRenderer == NULL) {
        ERROR(Txt::CouldNotCreateRendererContext, SDL_GetError());
        return false;
    }

    SDL_SetRenderDrawColor(sdlRenderer, 127, 127, 127, 255);
    SDL_RenderClear(sdlRenderer);

    DEBUG(Txt::InitializingSDLImage);

    auto imageTypes = IMG_INIT_PNG | IMG_INIT_JPG;

    if (!(IMG_Init(imageTypes) & imageTypes)) {
        ERROR(Txt::CouldNotInitializeSDLImage, IMG_GetError());
        return false;
    }

    DEBUG(Txt::InitializingSDLTTF);

    if (TTF_Init() != 0) {
        ERROR(Txt::CouldNotInitializeSDLTTF, TTF_GetError());
        return false;
    }

    DEBUG(Txt::LoadingDefaultFont, Renderer::DefaultFontPath);

    textFont = TTF_OpenFont(Renderer::DefaultFontPath, Renderer::TextSize);

    if (textFont == NULL) {
        ERROR(Txt::CouldNotLoadDefaultFont, TTF_GetError());
        return false;
    }

    DEBUG(Txt::Initialized);
    return true;
}

void Renderer::Finalize() {
    DEBUG(Txt::Finalizing);

    if (textFont != NULL) {
        DEBUG(Txt::UnloadingDefaultFont);
        TTF_CloseFont(textFont);
    }

    if (sdlRenderer != NULL) {
        DEBUG(Txt::DestroyingRendererContext);
        SDL_DestroyRenderer(sdlRenderer);
        sdlRenderer = NULL;
    }

    if (sdlWindow != NULL) {
        DEBUG(Txt::DestroyingRendererWindow);
        SDL_DestroyWindow(sdlWindow);
        sdlWindow = NULL;
    }

    TTF_Quit();
    IMG_Quit();
    SDL_QuitSubSystem(SDL_INIT_VIDEO);

    DEBUG(Txt::Finalized);
}

void Renderer::Update() {
    SDL_RenderPresent(sdlRenderer);
}

void Renderer::Splash(const Image* image) {
    if (image == NULL) {
        return;
    }

    auto imageTexture = (SDL_Texture*) image->data;
    SDL_RenderCopy(sdlRenderer, imageTexture, NULL, &windowRect);
}

void Renderer::Draw(const Image* image, const Vector2D& position, const Vector2D& size) {
    if (image == NULL) {
        return;
    }

    static SDL_Rect destinationRect;

    destinationRect.x = position.x;
    destinationRect.y = position.y;
    destinationRect.w = size.x;
    destinationRect.h = size.y;

    auto imageTexture = (SDL_Texture*) image->data;
    SDL_RenderCopy(sdlRenderer, imageTexture, NULL, &destinationRect);
}

Image* Renderer::LoadImage(const std::string& filePath) {
    auto imageSurface = IMG_Load(filePath.c_str());

    if (imageSurface == NULL) {
        WARNING(Txt::CouldNotLoadImage, filePath.c_str(), IMG_GetError());
        return NULL;
    }

    DEBUG(Txt::ImageLoaded, filePath.c_str());
    return ImageFromSurface(imageSurface);
}

Image* Renderer::ImageFromSurface(SDL_Surface* surface) {
    int imageWidth  = surface->w;
    int imageHeight = surface->h;

    SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(sdlRenderer, surface);
    SDL_FreeSurface(surface);

    if (imageTexture == NULL) {
        WARNING(Txt::CouldNotCreateImageTexture, SDL_GetError());
        return NULL;
    }

    auto image = new Image();

    image->width  = imageWidth;
    image->height = imageHeight;
    image->data   = imageTexture;

    return image;
}

void Renderer::UnloadImage(const Image* image) {
    if (image == NULL) {
        return;
    }

    SDL_DestroyTexture((SDL_Texture*) image->data);
    delete image;
}

Image* Renderer::TextImage(const std::string& text) {
    static SDL_Color textColor = {255, 255, 255, 255};

    auto textSurface = TTF_RenderText_Blended(textFont, text.c_str(), textColor);

    if (textSurface == NULL) {
        WARNING(Txt::CouldNotCreateTextTexture, SDL_GetError());
        return NULL;
    }

    return ImageFromSurface(textSurface);
}

}    // namespace Biq