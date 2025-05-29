//
// Created by Lucas N. Ferreira on 03/08/23.
//

#include "Player.h"

#include "../Components/ColliderComponents/AABBColliderComponent.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Game.h"

Player::Player(Game *game, const float forwardSpeed, const float jumpSpeed)
    : Actor(game),
      mForwardSpeed(forwardSpeed),
      mJumpSpeed(jumpSpeed),
      mIsRunning(false),
      mIsDead(false),
      mRigidBodyComponent(nullptr),
      mDrawComponent(nullptr),
      mColliderComponent(nullptr) {
    mRigidBodyComponent = new RigidBodyComponent(this, 1.0f, 5.0);

    mColliderComponent =
        new AABBColliderComponent(this, 0, 0, Game::TILE_SIZE - 4.0f,
                                  Game::TILE_SIZE, ColliderLayer::Player);

    mDrawComponent =
        new DrawAnimatedComponent(this, "../Assets/Sprites/Mario/texture.png",
                                  "../Assets/Sprites/Mario/texture.json");

    mDrawComponent->AddAnimation("dead", {0});
    mDrawComponent->AddAnimation("idle", {1});
    mDrawComponent->AddAnimation("jump", {2});
    mDrawComponent->AddAnimation("run", {3, 4, 5});

    mDrawComponent->SetAnimation("idle");
    mDrawComponent->SetAnimFPS(10.0f);
}

void Player::OnProcessInput(const uint8_t *keyState) {
    if (keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT]) {
        mRigidBodyComponent->ApplyForce(mForwardSpeed * Vector2(1, 0));
        mRotation = 0;
        mIsRunning = true;
    } else if (keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT]) {
        mRigidBodyComponent->ApplyForce(mForwardSpeed * Vector2(-1, 0));
        mRotation = Math::Pi;
        mIsRunning = true;
    } else {
        mIsRunning = false;
    }

    if ((keyState[SDL_SCANCODE_SPACE] || keyState[SDL_SCANCODE_W] ||
         keyState[SDL_SCANCODE_UP]) &&
        mIsOnGround) {
        mRigidBodyComponent->ApplyForce(mJumpSpeed * Vector2::UnitY);
        mIsOnGround = false;
    }
}

void Player::OnUpdate(float deltaTime) {
    if (mRigidBodyComponent && mRigidBodyComponent->GetVelocity().y != 0) {
        mIsOnGround = false;
    }

    float cameraX = mGame->GetCameraPos().x;
    if (mPosition.x < cameraX) {
        mPosition.x = cameraX;
    }

    if (mPosition.y > Game::LEVEL_HEIGHT * Game::TILE_SIZE) {
        Kill();
    }

    ManageAnimations();
}

void Player::ManageAnimations() {
    if (mIsDead) return mDrawComponent->SetAnimation("dead");

    if (mIsOnGround && mIsRunning) return mDrawComponent->SetAnimation("run");
    //  -5.1.2: Se estiver vivo, no chão e não estiver correndo, altere a
    //  animação para `idle`
    if (mIsOnGround) return mDrawComponent->SetAnimation("idle");
    //  -5.1.3: Se estiver vivo e não estiver no chão, altere a animação para
    //  `jump`
    mDrawComponent->SetAnimation("jump");
}

void Player::Kill() {
    // --------------
    // TODO - PARTE 5
    // --------------

    // TODO 8 (~4 linhas): altere a animação para "dead" e o valor da variável
    // `mIsDead` para verdadeiro.
    //  Além disso, desabilite os componentes `mRigidBodyComponent` e
    //  `mColliderComponent`
    mDrawComponent->SetAnimation("dead");
    mIsDead = true;
    mRigidBodyComponent->SetEnabled(false);
    mColliderComponent->SetEnabled(false);
}

void Player::OnHorizontalCollision(const float minOverlap,
                                   AABBColliderComponent *other) {
    // --------------
    // TODO - PARTE 5
    // --------------

    // TODO 9 (~2-4 linhas): Verifique se a colisão ocorreu horizontalmente com
    // um objeto do tipo
    //  `CollisionSide::Enemy`. Se sim, mate o jogador com o método `Kill()` do
    //  jogador.
    if (other->GetLayer() == ColliderLayer::Enemy) {
        Kill();
    }
}

void Player::OnVerticalCollision(const float minOverlap,
                                 AABBColliderComponent *other) {
    switch (other->GetLayer()) {
        case ColliderLayer::Enemy: {
            other->GetOwner()->Kill();
            auto vel = mRigidBodyComponent->GetVelocity();
            mRigidBodyComponent->SetVelocity(Vector2(vel.x, mJumpSpeed / 2));
            return;
        }
        case ColliderLayer::Blocks: {
            if (minOverlap > 0) return;
            other->GetOwner()->OnVerticalCollision(minOverlap,
                                                   mColliderComponent);
        }
    }
}
