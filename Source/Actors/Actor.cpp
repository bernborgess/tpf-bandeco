
#include "Actor.h"

#include <algorithm>

#include "../Components/Component.h"
#include "../Game.h"

Actor::Actor(Game* game)
    : mState(ActorState::Active),
      mPosition(Vector2::Zero),
      mScale(1.0f),
      mRotation(0.0f),
      mGame(game) {
    mGame->AddActor(this);
}

Actor::~Actor() {
    mGame->RemoveActor(this);

    for (auto component : mComponents) {
        delete component;
    }
    mComponents.clear();
}

void Actor::SetPosition(const Vector2& pos) {
    mPosition = pos;
    mGame->Reinsert(this);
}

void Actor::Update(float deltaTime) {
    if (mState == ActorState::Active) {
        for (auto comp : mComponents) {
            if (comp->IsEnabled()) {
                comp->Update(deltaTime);
            }
        }

        OnUpdate(deltaTime);
    }
}

void Actor::OnUpdate(float deltaTime) {}

void Actor::OnHorizontalCollision(const float minOverlap,
                                  AABBColliderComponent* other) {}

void Actor::OnVerticalCollision(const float minOverlap,
                                AABBColliderComponent* other) {}

void Actor::Kill() {}

void Actor::ProcessInput(const Uint8* keyState) {
    if (mState == ActorState::Active) {
        for (auto comp : mComponents) {
            comp->ProcessInput(keyState);
        }

        OnProcessInput(keyState);
    }
}

void Actor::HandleKeyPress(const int scanCode, const bool isPressed) {
    if (mState == ActorState::Active) {
        for (auto comp : mComponents) {
            comp->HandleKeyPress(scanCode, isPressed);
        }

        // Call actor-specific key press handling
        OnHandleKeyPress(scanCode, isPressed);
    }
}

void Actor::OnProcessInput(const Uint8* keyState) {}

void Actor::OnHandleKeyPress(const int scanCode, const bool isPressed) {}

void Actor::AddComponent(Component* c) {
    mComponents.emplace_back(c);
    std::sort(mComponents.begin(), mComponents.end(),
              [](Component* a, Component* b) {
                  return a->GetUpdateOrder() < b->GetUpdateOrder();
              });
}

bool Actor::IsVisibleOnCamera() const {
    // Get the camera's position and dimensions
    Vector2 cameraPosition = mGame->GetCameraPos();
    float screenWidth = mGame->GetWindowWidth();
    float screenHeight = mGame->GetWindowHeight();

    // Check if the actor's position is within the camera's view
    return (mPosition.x >= cameraPosition.x &&
            mPosition.x <= cameraPosition.x + screenWidth &&
            mPosition.y >= cameraPosition.y &&
            mPosition.y <= cameraPosition.y + screenHeight);
}
