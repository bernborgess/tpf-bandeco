//
// Created by Lucas N. Ferreira on 08/09/23.
//

#pragma once
#include <SDL2/SDL_scancode.h>

#include "../Game.h"
#include "../Level.h"
#include "Actor.h"
#include "Item.h"

enum class PlayerType { PlayerB, PlayerD };
enum class FaceDirection { North, East, South, West };

class Player : public Actor {
   public:
    static const int PLAYER_WIDTH = 28;
    static const int PLAYER_HEIGHT = 32;

    explicit Player(Game* game, PlayerType playerType,
                    float forwardSpeed = 1500.0f, float jumpSpeed = -750.0f);

    void OnProcessInput(const Uint8* keyState) override;
    void OnUpdate(float deltaTime) override;
    void OnHandleKeyPress(const int key, const bool isPressed) override;

    void OnHorizontalCollision(const float minOverlap,
                               AABBColliderComponent* other) override;
    void OnVerticalCollision(const float minOverlap,
                             AABBColliderComponent* other) override;

   private:
    void ManageAnimations();

    PlayerType mPlayerType;
    // Manage different players
    SDL_Scancode GetDownCode();
    SDL_Scancode GetLeftCode();
    SDL_Scancode GetRightCode();
    SDL_Scancode GetUpCode();
    SDL_Scancode GetPickUpCode();
    SDL_Scancode GetChopCode();
    SDL_Scancode GetDashCode();

    float mForwardSpeed;
    bool mIsRunning;
    FaceDirection mFaceDirection;

    Item* mHandItem;
    std::tuple<LevelDataEntry, int, int> GetFocusBlock();
    void HandlePickUp();
    void HandlePutDown();
    void HandleChop();

    class RigidBodyComponent* mRigidBodyComponent;
    class DrawAnimatedComponent* mDrawComponent;
    class AABBColliderComponent* mColliderComponent;
};