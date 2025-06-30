//
// Created by Lucas N. Ferreira on 03/08/23.
//

#include "DrawComponent.h"

#include "../../Actors/Actor.h"
#include "../../Game.h"

DrawComponent::DrawComponent(class Actor* owner, int drawOrder)
    : Component(owner),
      mDrawOrder(drawOrder),
      mIsVisible(true),
      mIsAbove(false) {
    //    mOwner->GetGame()->AddDrawable(this);
}

DrawComponent::~DrawComponent() {
    //    mOwner->GetGame()->RemoveDrawable(this);
}

void DrawComponent::Draw(SDL_Renderer* renderer, const Vector3& modColor) {}

void DrawComponent::ResetAbove() {
    if (mIsAbove) {
        mDrawOrder /= 10;
        mIsAbove = false;
    }
}

void DrawComponent::SetAbove() {
    if (!mIsAbove) {
        mDrawOrder *= 10;
        mIsAbove = true;
    }
}