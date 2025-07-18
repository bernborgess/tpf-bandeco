
#include "Component.h"

#include "../Actors/Actor.h"

Component::Component(Actor* owner, int updateOrder)
    : mOwner(owner), mUpdateOrder(updateOrder), mIsEnabled(true) {
    // Add to actor's vector of components
    mOwner->AddComponent(this);
}

Component::~Component() {}

void Component::Update(float deltaTime) {}

void Component::ProcessInput(const Uint8* keyState) {}

void Component::HandleKeyPress(const int scanCode, bool isPressed) {}

class Game* Component::GetGame() const { return mOwner->GetGame(); }
