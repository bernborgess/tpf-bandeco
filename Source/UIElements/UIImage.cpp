//
// Created by Lucas N. Ferreira on 28/05/25.
//

#include "UIImage.h"

#include "../Game.h"

UIImage::UIImage(Game *game, const std::string &imagePath, const Vector2 &pos,
                 const Vector2 &size, const Vector3 &color)
    : UIElement(pos, size, color), mTexture(nullptr) {
    SDL_Surface *surface = IMG_Load(imagePath.c_str());

    if (!surface) {
        SDL_Log("Failed to load image: %s", IMG_GetError());
        return;
    }

    mTexture = SDL_CreateTextureFromSurface(game->GetRenderer(), surface);
    SDL_FreeSurface(surface);

    if (!mTexture) {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
        return;
    }
}

UIImage::~UIImage() {
    if (mTexture == nullptr) return;
    SDL_DestroyTexture(mTexture);
    mTexture = nullptr;
}

void UIImage::Draw(SDL_Renderer *renderer, const Vector2 &screenPos) {
    if (mTexture == nullptr) return;

    Vector2 imagePosition = screenPos + mPosition;

    SDL_Rect imageRect = {.x = (int)imagePosition.x,
                          .y = (int)imagePosition.y,
                          .w = (int)mSize.x,
                          .h = (int)mSize.y};

    SDL_RenderCopy(renderer, mTexture, nullptr, &imageRect);
}