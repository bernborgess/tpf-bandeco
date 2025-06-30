#include "UIFont.h"

#include <SDL_image.h>

#include <vector>

UIFont::UIFont(SDL_Renderer *renderer) : mRenderer(renderer) {}

UIFont::~UIFont() {}

bool UIFont::Load(const std::string &fileName) {
    // We support these font sizes
    std::vector<int> fontSizes = {8,  9,  10, 11, 12, 14, 16, 18, 20, 22,
                                  24, 26, 28, 30, 32, 34, 36, 38, 40, 42,
                                  44, 46, 48, 52, 56, 60, 64, 68, 72};

    for (auto &ptsize : fontSizes) {
        TTF_Font *font = TTF_OpenFont(fileName.c_str(), ptsize);
        if (!font) {
            return false;
        }
        mFontData[ptsize] = font;
    }

    return true;
}

void UIFont::Unload() {
    for (auto &[ptsize, font] : mFontData) {
        TTF_CloseFont(font);
    }
    mFontData.clear();
}

SDL_Texture *UIFont::RenderText(const std::string &text,
                                const Vector3 &color /*= Color::White*/,
                                int pointSize /*= 24*/,
                                unsigned wrapLength /*= 900*/) {
    if (text.length() <= 0) {
        // Invalid text
        return nullptr;
    }
    if (!mRenderer) {
        SDL_Log("Renderer is null. Can't Render Text!");
        return nullptr;
    }

    // Convert to SDL_Color
    SDL_Color sdlColor;

    // Swap red and blue so we get RGBA instead of BGRA
    sdlColor.r = static_cast<Uint8>(color.x);
    sdlColor.g = static_cast<Uint8>(color.y);
    sdlColor.b = static_cast<Uint8>(color.z);
    sdlColor.a = 255;

    if (mFontData.count(pointSize) == 0) {
        SDL_Log("pointSize %d is not supported!", pointSize);
        return nullptr;
    }

    TTF_Font *font = mFontData[pointSize];
    SDL_Surface *surface = TTF_RenderUTF8_Blended_Wrapped(font, text.c_str(),
                                                          sdlColor, wrapLength);

    SDL_Texture *texture = SDL_CreateTextureFromSurface(mRenderer, surface);
    if (!texture) {
        SDL_Log("Texture could not be created for text \"%s\" of length %d!",
                text.c_str(), text.length());
        return nullptr;
    }
    return texture;
}
