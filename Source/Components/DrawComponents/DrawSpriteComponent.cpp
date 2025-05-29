//
// Created by Lucas N. Ferreira on 28/09/23.
//

#include "DrawSpriteComponent.h"
#include "../../Actors/Actor.h"
#include "../../Game.h"

DrawSpriteComponent::DrawSpriteComponent(class Actor *owner, const std::string &texturePath, const int width,
                                         const int height, const int drawOrder)
    : DrawComponent(owner, drawOrder)
      , mWidth(width)
      , mHeight(height) {
    mSpriteSheetSurface = owner->GetGame()->LoadTexture(texturePath);
}

void DrawSpriteComponent::Draw(SDL_Renderer *renderer) {
    Vector2 pos = mOwner->GetPosition(),
            cam = mOwner->GetGame()->GetCameraPos();
    SDL_Rect rect = {
        .x = int(pos.x - cam.x),
        .y = int(pos.y - cam.y),
        .w = mWidth,
        .h = mHeight
    };
    auto rot = mOwner->GetRotation();
    auto flip = Math::NearZero(rot) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;

    SDL_RenderCopyEx(
        renderer,
        mSpriteSheetSurface,
        nullptr,
        &rect,
        0.0,
        nullptr,
        flip
    );
}
