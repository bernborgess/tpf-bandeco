//
// Created by Lucas N. Ferreira on 03/08/23.
//

#include "Player.h"

#include "../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"
#include "../Game.h"
#include "Block.h"

Player::Player(Game *game, const float forwardSpeed, const float jumpSpeed)
    : Actor(game),
      mIsRunning(false),
      mIsOnPole(false),
      mIsDying(false),
      mForwardSpeed(forwardSpeed),
      mJumpSpeed(jumpSpeed),
      mPoleSlideTimer(0.0f) {
    mRigidBodyComponent = new RigidBodyComponent(this, 1.0f, 5.0f);
    mColliderComponent =
        new AABBColliderComponent(this, 0, 0, Game::TILE_SIZE - 4.0f,
                                  Game::TILE_SIZE, ColliderLayer::Player);

    mDrawComponent =
        new DrawAnimatedComponent(this, "../Assets/Sprites/Player/Player.png",
                                  "../Assets/Sprites/Player/Player.json");

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
    if (state[SDL_SCANCODE_D]) {
        mRigidBodyComponent->ApplyForce(Vector2::UnitX * mForwardSpeed);
        mRotation = 0.0f;
        mIsRunning = true;
    }

    if (state[SDL_SCANCODE_A]) {
        mRigidBodyComponent->ApplyForce(Vector2::UnitX * -mForwardSpeed);
        mRotation = Math::Pi;
        mIsRunning = true;
    }

    if (state[SDL_SCANCODE_W]) {
        mRigidBodyComponent->ApplyForce(Vector2::UnitY * -mForwardSpeed);
        mIsRunning = true;
    }

    if (state[SDL_SCANCODE_S]) {
        mRigidBodyComponent->ApplyForce(Vector2::UnitY * mForwardSpeed);
        mIsRunning = true;
    }
}

void Player::OnHandleKeyPress(const int key, const bool isPressed) {
    if (mGame->GetGamePlayState() != Game::GamePlayState::Playing) return;
}

void Player::OnUpdate(float deltaTime) {
    // Limit Player's position to the camera view
    mPosition.x = Math::Max(mPosition.x, mGame->GetCameraPos().x);

    // Kill player if he falls below the screen
    if (mGame->GetGamePlayState() == Game::GamePlayState::Playing &&
        mPosition.y > mGame->GetWindowHeight()) {
        Kill();
    }

    if (mIsOnPole) {
        // If Player is on the pole, update the pole slide timer
        mPoleSlideTimer -= deltaTime;
        if (mPoleSlideTimer <= 0.0f) {
            mRigidBodyComponent->SetApplyFriction(false);
            mRigidBodyComponent->SetVelocity(Vector2::UnitX * 100.0f);
            mGame->SetGamePlayState(Game::GamePlayState::Leaving);

            mGame->GetAudio()->PlaySound("StageClear.wav");

            mIsOnPole = false;
            mIsRunning = true;
        }
    }

    // If Player is leaving the level, kill him if he enters the castle
    const float castleDoorPos =
        Game::LEVEL_WIDTH * Game::TILE_SIZE - 10 * Game::TILE_SIZE;

    if (mGame->GetGamePlayState() == Game::GamePlayState::Leaving &&
        mPosition.x >= castleDoorPos) {
        // Stop Player and set the game scene to Level 2
        mState = ActorState::Destroy;
        mGame->SetGameScene(Game::GameScene::Level2, 3.5f);

        return;
    }

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
    } else if (other->GetLayer() == ColliderLayer::Blocks) {
        mGame->GetAudio()->PlaySound("Bump.wav");

        // Cast actor to Block to call OnBump
        auto *block = dynamic_cast<Block *>(other->GetOwner());
        block->OnBump();
    }
}