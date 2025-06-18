//
// Created by Lucas N. Ferreira on 08/09/23.
//

#pragma once
#include "Actor.h"

class Player : public Actor {
   public:
    static const int PLAYER_WIDTH = 28;
    static const int PLAYER_HEIGHT = 32;

    explicit Player(Game* game, float forwardSpeed = 1500.0f,
                    float jumpSpeed = -750.0f);

    void OnProcessInput(const Uint8* keyState) override;
    void OnUpdate(float deltaTime) override;
    void OnHandleKeyPress(const int key, const bool isPressed) override;

    void OnHorizontalCollision(const float minOverlap,
                               AABBColliderComponent* other) override;
    void OnVerticalCollision(const float minOverlap,
                             AABBColliderComponent* other) override;

    void Kill() override;
    void Win(AABBColliderComponent* poleCollider);

   private:
    static const int POLE_SLIDE_TIME =
        1;  // Time in seconds to slide down the pole

    void ManageAnimations();

    float mForwardSpeed;
    float mJumpSpeed;
    float mPoleSlideTimer;
    bool mIsRunning;
    bool mIsOnPole;
    bool mIsDying;

    class RigidBodyComponent* mRigidBodyComponent;
    class DrawAnimatedComponent* mDrawComponent;
    class AABBColliderComponent* mColliderComponent;
};