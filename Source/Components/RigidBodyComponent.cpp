//
// Created by Lucas N. Ferreira on 08/09/23.
//

#include "RigidBodyComponent.h"

#include <SDL.h>

#include "../Actors/Actor.h"
#include "../Game.h"
#include "ColliderComponents/AABBColliderComponent.h"

const float MAX_SPEED_X = 750.0f;
const float MAX_SPEED_Y = 750.0f;

RigidBodyComponent::RigidBodyComponent(class Actor* owner, float mass,
                                       float friction, int updateOrder)
    : Component(owner, updateOrder),
      mMass(mass),
      mApplyFriction(true),
      mFrictionCoefficient(friction),
      mVelocity(Vector2::Zero),
      mAcceleration(Vector2::Zero) {}

void RigidBodyComponent::ApplyForce(const Vector2& force) {
    mAcceleration += force * (1.f / mMass);
}

void RigidBodyComponent::Update(float deltaTime) {
    // Apply friction
    if (mApplyFriction && Math::Abs(mVelocity.x) > 0.05f) {
        ApplyForce(Vector2::UnitX * -mFrictionCoefficient * mVelocity.x);
    }
    if (mApplyFriction && Math::Abs(mVelocity.y) > 0.05f) {
        ApplyForce(Vector2::UnitY * -mFrictionCoefficient * mVelocity.y);
    }

    // Euler Integration
    mVelocity += mAcceleration * deltaTime;

    mVelocity.x = Math::Clamp<float>(mVelocity.x, -MAX_SPEED_X, MAX_SPEED_X);
    mVelocity.y = Math::Clamp<float>(mVelocity.y, -MAX_SPEED_Y, MAX_SPEED_Y);

    if (Math::NearZero(mVelocity.x, 1.0f)) {
        mVelocity.x = 0.f;
    }

    auto collider = mOwner->GetComponent<AABBColliderComponent>();

    if (mVelocity.x != 0.0f) {
        mOwner->SetPosition(
            Vector2(mOwner->GetPosition().x + mVelocity.x * deltaTime,
                    mOwner->GetPosition().y));

        if (collider) {
            collider->DetectHorizontalCollision(this);
        }
    }

    if (mVelocity.y != 0.0f) {
        mOwner->SetPosition(
            Vector2(mOwner->GetPosition().x,
                    mOwner->GetPosition().y + mVelocity.y * deltaTime));

        if (collider) {
            collider->DetectVerticalCollision(this);
        }
    }

    mAcceleration.Set(0.f, 0.f);
}
