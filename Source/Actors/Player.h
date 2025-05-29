//
// Created by Lucas N. Ferreira on 08/09/23.
//

#pragma once
#include "Actor.h"

class Player : public Actor {
   public:
    explicit Player(Game* game, float forwardSpeed = 1500.0f,
                    float jumpSpeed = -75000.0f);

    void OnProcessInput(const Uint8* keyState) override;
    void OnUpdate(float deltaTime) override;

    void OnHorizontalCollision(const float minOverlap,
                               AABBColliderComponent* other) override;
    void OnVerticalCollision(const float minOverlap,
                             AABBColliderComponent* other) override;

    void Kill() override;

   private:
    void ManageAnimations();

    float mForwardSpeed;
    float mJumpSpeed;
    bool mIsRunning;
    bool mIsDead;

    class RigidBodyComponent* mRigidBodyComponent;
    class DrawAnimatedComponent* mDrawComponent;
    class AABBColliderComponent* mColliderComponent;
};