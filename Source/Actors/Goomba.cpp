//
// Created by Lucas N. Ferreira on 30/09/23.
//

#include "Goomba.h"

#include "../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"
#include "../Game.h"
#include "../Random.h"

Goomba::Goomba(Game* game, float forwardSpeed, float deathTime)
    : Actor(game),
      mDyingTimer(deathTime),
      mIsDying(false),
      mForwardSpeed(forwardSpeed) {
    mRigidBodyComponent = new RigidBodyComponent(this, 1.0f);
    mRigidBodyComponent->SetVelocity(Vector2(-mForwardSpeed, 0.0f));

    mColliderComponent = new AABBColliderComponent(
        this, 0, 0, Game::TILE_SIZE, Game::TILE_SIZE, ColliderLayer::Enemy);

    mDrawComponent =
        new DrawAnimatedComponent(this, "../Assets/Sprites/Goomba/Goomba.png",
                                  "../Assets/Sprites/Goomba/Goomba.json");

    mDrawComponent->AddAnimation("Dead", {0});
    mDrawComponent->AddAnimation("Idle", {1});
    mDrawComponent->AddAnimation("walk", {1, 2});
    mDrawComponent->SetAnimation("walk");
    mDrawComponent->SetAnimFPS(5.0f);
}

void Goomba::Kill() {
    mIsDying = true;
    mDrawComponent->SetAnimation("Dead");
    mRigidBodyComponent->SetEnabled(false);
    mColliderComponent->SetEnabled(false);
}

void Goomba::BumpKill(const float bumpForce) {
    mDrawComponent->SetAnimation("Idle");

    mRigidBodyComponent->SetVelocity(Vector2(bumpForce / 2.0f, -bumpForce));
    mColliderComponent->SetEnabled(false);

    // Flip upside down (180 degrees)
    SetRotation(180);
}

void Goomba::OnUpdate(float deltaTime) {
    if (mIsDying) {
        mDyingTimer -= deltaTime;
        if (mDyingTimer <= 0.0f) {
            mState = ActorState::Destroy;
        }
    }

    if (GetPosition().y > GetGame()->GetWindowHeight()) {
        mState = ActorState::Destroy;
    }
}

void Goomba::OnHorizontalCollision(const float minOverlap,
                                   AABBColliderComponent* other) {
    if ((other->GetLayer() == ColliderLayer::Blocks ||
         other->GetLayer() == ColliderLayer::Enemy)) {
        if (minOverlap > 0) {
            mRigidBodyComponent->SetVelocity(Vector2(-mForwardSpeed, 0.0f));
        } else {
            mRigidBodyComponent->SetVelocity(Vector2(mForwardSpeed, 0.0f));
        }
    }

    if (other->GetLayer() == ColliderLayer::Player) {
        other->GetOwner()->Kill();
    }
}

void Goomba::OnVerticalCollision(const float minOverlap,
                                 AABBColliderComponent* other) {
    if (other->GetLayer() == ColliderLayer::Player) {
        other->GetOwner()->Kill();
    }
}
