//
// Created by Lucas N. Ferreira on 03/08/23.
//

#include "Player.h"

#include "../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"
#include "../Game.h"
#include "Block.h"

Player::Player(Game *game, const PlayerType playerType,
               const float forwardSpeed, const float jumpSpeed)
    : Actor(game),
      mIsRunning(false),
      mIsOnPole(false),
      mIsDying(false),
      mPlayerType(playerType),
      mForwardSpeed(forwardSpeed),
      mJumpSpeed(jumpSpeed),
      mPoleSlideTimer(0.0f) {
    mRigidBodyComponent = new RigidBodyComponent(this, 1.0f, 5.0f);
    mColliderComponent = new AABBColliderComponent(
        this, 0, 0, PLAYER_WIDTH, PLAYER_HEIGHT, ColliderLayer::Player);

    mDrawComponent = mPlayerType == PlayerType::PlayerB
                         ? new DrawAnimatedComponent(
                               this, "../Assets/Sprites/PlayerB/Player.png",
                               "../Assets/Sprites/PlayerB/Player.json")
                         : new DrawAnimatedComponent(
                               this, "../Assets/Sprites/PlayerD/Player.png",
                               "../Assets/Sprites/PlayerD/Player.json");

    mDrawComponent->AddAnimation("Dead", {0});
    mDrawComponent->AddAnimation("idle", {1});
    mDrawComponent->AddAnimation("jump", {2});
    mDrawComponent->AddAnimation("run", {3, 4, 5});
    mDrawComponent->AddAnimation("win", {7});

    mDrawComponent->SetAnimation("idle");
    mDrawComponent->SetAnimFPS(10.0f);
}

void Player::OnProcessInput(const uint8_t *state) {
    if (mGame->GetGamePlayState() != Game::GamePlayState::Playing) return;

    mIsRunning = false;
    if (state[GetRightCode()]) {
        mRigidBodyComponent->ApplyForce(Vector2::UnitX * mForwardSpeed);
        mRotation = 0.0f;
        mIsRunning = true;
    }

    if (state[GetLeftCode()]) {
        mRigidBodyComponent->ApplyForce(Vector2::UnitX * -mForwardSpeed);
        mRotation = Math::Pi;
        mIsRunning = true;
    }

    if (state[GetUpCode()]) {
        mRigidBodyComponent->ApplyForce(Vector2::UnitY * -mForwardSpeed);
        mIsRunning = true;
    }

    if (state[GetDownCode()]) {
        mRigidBodyComponent->ApplyForce(Vector2::UnitY * mForwardSpeed);
        mIsRunning = true;
    }
}

void Player::OnHandleKeyPress(const int scanCode, const bool isPressed) {
    if (mGame->GetGamePlayState() != Game::GamePlayState::Playing) return;

    if (scanCode == GetActionCode()) {
        SDL_Log("Player %d ACTION", mPlayerType);
    }
    if (scanCode == GetUseCode()) {
        SDL_Log("Player %d USE", mPlayerType);
    }
    if (scanCode == GetImpulseCode()) {
        SDL_Log("Player %d IMPULSE", mPlayerType);
    }
}

void Player::OnUpdate(float deltaTime) {
    // TODO: Check win and set the game scene to Level 2
    // mState = ActorState::Destroy;
    // mGame->SetGameScene(Game::GameScene::Level2, 3.5f);

    ManageAnimations();
}

void Player::ManageAnimations() {
    if (mIsDying) {
        mDrawComponent->SetAnimation("Dead");
    } else if (mIsOnPole) {
        mDrawComponent->SetAnimation("win");
    } else if (mIsRunning) {
        mDrawComponent->SetAnimation("run");
    } else {
        mDrawComponent->SetAnimation("idle");
    }
}

void Player::Kill() {
    mIsDying = true;
    mGame->SetGamePlayState(Game::GamePlayState::GameOver);
    mDrawComponent->SetAnimation("Dead");

    // Disable collider and rigid body
    mRigidBodyComponent->SetEnabled(false);
    mColliderComponent->SetEnabled(false);

    mGame->GetAudio()->StopAllSounds();
    mGame->GetAudio()->PlaySound("Dead.wav");

    mGame->ResetGameScene(3.5f);  // Reset the game scene after 3 seconds
}

void Player::Win(AABBColliderComponent *poleCollider) {
    mDrawComponent->SetAnimation("win");
    mGame->SetGamePlayState(Game::GamePlayState::LevelComplete);

    // Set player velocity to go down
    mRigidBodyComponent->SetVelocity(Vector2::UnitY *
                                     100.0f);  // 100 pixels per second

    // Disable collider
    poleCollider->SetEnabled(false);

    // Adjust player x position to grab the pole
    mPosition.Set(
        poleCollider->GetOwner()->GetPosition().x + Game::TILE_SIZE / 4.0f,
        mPosition.y);

    mGame->GetAudio()->StopAllSounds();

    mPoleSlideTimer = POLE_SLIDE_TIME;  // Start the pole slide timer
}

void Player::OnHorizontalCollision(const float minOverlap,
                                   AABBColliderComponent *other) {
    if (other->GetLayer() == ColliderLayer::Enemy) {
        Kill();
    } else if (other->GetLayer() == ColliderLayer::Pole) {
        mIsOnPole = true;
        Win(other);
    }
}

void Player::OnVerticalCollision(const float minOverlap,
                                 AABBColliderComponent *other) {
    if (other->GetLayer() == ColliderLayer::Enemy) {
        other->GetOwner()->Kill();
        mRigidBodyComponent->SetVelocity(
            Vector2(mRigidBodyComponent->GetVelocity().x, mJumpSpeed / 2.5f));

        mGame->GetAudio()->PlaySound("Stomp.wav");
    }
}

SDL_Scancode Player::GetDownCode() {
    switch (mPlayerType) {
        case PlayerType::PlayerB:
            return SDL_SCANCODE_S;
        case PlayerType::PlayerD:
            return SDL_SCANCODE_DOWN;
    }
}

SDL_Scancode Player::GetLeftCode() {
    switch (mPlayerType) {
        case PlayerType::PlayerB:
            return SDL_SCANCODE_A;
        case PlayerType::PlayerD:
            return SDL_SCANCODE_LEFT;
    }
}

SDL_Scancode Player::GetRightCode() {
    switch (mPlayerType) {
        case PlayerType::PlayerB:
            return SDL_SCANCODE_D;
        case PlayerType::PlayerD:
            return SDL_SCANCODE_RIGHT;
    }
}

SDL_Scancode Player::GetUpCode() {
    switch (mPlayerType) {
        case PlayerType::PlayerB:
            return SDL_SCANCODE_W;
        case PlayerType::PlayerD:
            return SDL_SCANCODE_UP;
    }
}

SDL_Scancode Player::GetActionCode() {
    switch (mPlayerType) {
        case PlayerType::PlayerB:
            return SDL_SCANCODE_LSHIFT;
        case PlayerType::PlayerD:
            return SDL_SCANCODE_RSHIFT;
    }
}

SDL_Scancode Player::GetUseCode() {
    switch (mPlayerType) {
        case PlayerType::PlayerB:
            return SDL_SCANCODE_LCTRL;
        case PlayerType::PlayerD:
            return SDL_SCANCODE_RCTRL;
    }
}

SDL_Scancode Player::GetImpulseCode() {
    switch (mPlayerType) {
        case PlayerType::PlayerB:
            return SDL_SCANCODE_LALT;
        case PlayerType::PlayerD:
            return SDL_SCANCODE_RALT;
    }
}
