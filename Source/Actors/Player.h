//
// Created by Lucas N. Ferreira on 08/09/23.
//

#pragma once
#include <SDL2/SDL_scancode.h>

#include "Actor.h"

enum PlayerType { PlayerMario, PlayerLuigi };

class Player : public Actor {
   public:
    explicit Player(Game* game, PlayerType playerType = PlayerMario,
                    float forwardSpeed = 1500.0f, float jumpSpeed = -75000.0f);

    void OnProcessInput(const Uint8* keyState) override;
    void OnUpdate(float deltaTime) override;

    void OnHorizontalCollision(const float minOverlap,
                               AABBColliderComponent* other) override;
    void OnVerticalCollision(const float minOverlap,
                             AABBColliderComponent* other) override;

    void Kill() override;

    PlayerType mPlayerType;

   private:
    void ManageAnimations();

    float mForwardSpeed;
    float mJumpSpeed;
    bool mIsRunning;
    bool mIsDead;

    class RigidBodyComponent* mRigidBodyComponent;
    class DrawAnimatedComponent* mDrawComponent;
    class AABBColliderComponent* mColliderComponent;

    // Manage different players
    SDL_Scancode GetDownCode();
    SDL_Scancode GetLeftCode();
    SDL_Scancode GetRightCode();
    SDL_Scancode GetUpCode();
};