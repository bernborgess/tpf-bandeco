#include "Player.h"

#include "../Blocks/Block.h"
#include "../Blocks/Deliver.h"
#include "../Blocks/FoodBox.h"
#include "../Blocks/Stove.h"
#include "../Blocks/Table.h"
#include "../Blocks/TableCut.h"
#include "../Blocks/Trash.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"
#include "../Game.h"
#include "Item.h"

Player::Player(Game *game, const PlayerType playerType,
               const float forwardSpeed, const float jumpSpeed)
    : Actor(game),
      mIsRunning(false),
      mPlayerType(playerType),
      mForwardSpeed(forwardSpeed),
      mHandItem(nullptr),
      mFaceDirection(FaceDirection::South) {
    mRigidBodyComponent = new RigidBodyComponent(this, 1.0f, 5.0f);

    switch (mPlayerType) {
        case PlayerType::PlayerB: {
            mColliderComponent = new AABBColliderComponent(
                this, 0, 0, 28, 40, ColliderLayer::Player);

            mDrawComponent = new DrawAnimatedComponent(
                this, "../Assets/Sprites/PlayerB/PlayerB.png",
                "../Assets/Sprites/PlayerB/PlayerB.json", 400);
            break;
        }
        case PlayerType::PlayerD: {
            mColliderComponent = new AABBColliderComponent(
                this, 0, 0, 28, 28, ColliderLayer::Player);

            mDrawComponent = new DrawAnimatedComponent(
                this, "../Assets/Sprites/PlayerD/PlayerD.png",
                "../Assets/Sprites/PlayerD/PlayerD.json", 400);
            break;
        }
    }

    mDrawComponent->AddAnimation("IdleBack", {0});
    mDrawComponent->AddAnimation("IdleFront", {1});
    mDrawComponent->AddAnimation("IdleRight", {2});
    mDrawComponent->AddAnimation("WalkBack", {3, 0, 4});
    mDrawComponent->AddAnimation("WalkFront", {5, 1, 6});
    mDrawComponent->AddAnimation("WalkRight", {7, 2, 8});

    mDrawComponent->SetAnimation("IdleFront");
    mDrawComponent->SetAnimFPS(10.0f);
}

void Player::OnProcessInput(const uint8_t *state) {
    if (mGame->GetGamePlayState() != Game::GamePlayState::Playing) return;

    mIsRunning = false;
    if (state[GetRightCode()]) {
        mRigidBodyComponent->ApplyForce(Vector2::UnitX * mForwardSpeed);
        mRotation = 0.0f;
        mFaceDirection = FaceDirection::East;
        mIsRunning = true;
    }

    if (state[GetLeftCode()]) {
        mRigidBodyComponent->ApplyForce(Vector2::UnitX * -mForwardSpeed);
        mRotation = Math::Pi;
        mFaceDirection = FaceDirection::West;
        mIsRunning = true;
    }

    if (state[GetUpCode()]) {
        mRigidBodyComponent->ApplyForce(Vector2::UnitY * -mForwardSpeed);
        mFaceDirection = FaceDirection::North;
        mIsRunning = true;
    }

    if (state[GetDownCode()]) {
        mRigidBodyComponent->ApplyForce(Vector2::UnitY * mForwardSpeed);
        mFaceDirection = FaceDirection::South;
        mIsRunning = true;
    }
}

void Player::OnHandleKeyPress(const int scanCode, const bool isPressed) {
    if (mGame->GetGamePlayState() != Game::GamePlayState::Playing) return;

    if (scanCode == GetPickUpCode()) {
        if (mHandItem == nullptr) {
            HandlePickUp();
        } else {
            HandlePutDown();
        }
    }
    if (scanCode == GetChopCode()) {
        HandleChop();
    }
    if (scanCode == GetDashCode()) {
        SDL_Log("Player %d DASH", (int)mPlayerType);
    }
}

std::pair<LevelTile, Block *> Player::GetFocusBlock() {
    // Calculate which block this player faces
    auto [px, py] = mPosition;
    int pxg = (px + 16) / 64;
    int pyg = (py + 16) / 64;

    // Add the direction the player is facing
    switch (mFaceDirection) {
        case FaceDirection::North:
            pyg--;
            break;
        case FaceDirection::East:
            pxg++;
            break;
        case FaceDirection::South:
            pyg++;
            break;
        case FaceDirection::West:
            pxg--;
            break;
    }

    // We look at the level data
    return mGame->GetLevelTileAt(pxg, pyg);
}

// Assumes player has empty hand
void Player::HandlePickUp() {
    if (mHandItem != nullptr) return;
    auto [levelEntry, block] = GetFocusBlock();
    if (block == nullptr) {
        SDL_Log("Expected a table, didn't find it!");
        return;
    }
    Item *item = block->PickItemOnTop();
    if (item) {
        mHandItem = item;
    }
}

// Assumes player has item in hand
void Player::HandlePutDown() {
    if (mHandItem == nullptr) return;
    const auto [levelEntry, block] = GetFocusBlock();
    if (block == nullptr) {  // Expected a block, didn't find it!
        return;
    }
    mHandItem = block->SetItemOnTop(mHandItem);
}

// If it's a TableCut and there's food on it, will chop
void Player::HandleChop() {
    if (mHandItem != nullptr) return;
    const auto [levelEntry, block] = GetFocusBlock();
    if (levelEntry != LevelTile::TileTableCut) return;
    if (block == nullptr) {
        SDL_Log("Expected a tableCut, didn't find it!");
        return;
    }
    TableCut *tableCut = (TableCut *)block;
    tableCut->OnItemCut();
}

void Player::OnUpdate(float deltaTime) {
    // TODO: Check win and set the game scene to Level 2
    // mState = ActorState::Destroy;
    // mGame->SetGameScene(Game::GameScene::Level2, 3.5f);

    // Hand Item follows player
    if (mHandItem != nullptr) {
        Vector2 offset;
        switch (mPlayerType) {
            case PlayerType::PlayerB: {
                switch (mFaceDirection) {
                    case FaceDirection::East: {
                        offset = Vector2(16, -16);
                        break;
                    }
                    case FaceDirection::West: {
                        offset = Vector2(-48, -16);
                        break;
                    }
                    case FaceDirection::South: {
                        offset = Vector2(-16, 16);
                        break;
                    }
                    case FaceDirection::North: {
                        offset = Vector2(-16, -80);
                        break;
                    }
                }
                break;
            }
            case PlayerType::PlayerD: {
                switch (mFaceDirection) {
                    case FaceDirection::East: {
                        offset = Vector2(16, -32);
                        break;
                    }
                    case FaceDirection::West: {
                        offset = Vector2(-48, -32);
                        break;
                    }
                    case FaceDirection::South: {
                        offset = Vector2(-16, 0);
                        break;
                    }
                    case FaceDirection::North: {
                        offset = Vector2(-16, -80);
                        break;
                    }
                }
                break;
            }
        }

        mHandItem->SetPosition(mPosition + offset);
    }

    ManageAnimations();
}

void Player::ManageAnimations() {
    switch (mFaceDirection) {
        case FaceDirection::East:
        case FaceDirection::West: {
            return mDrawComponent->SetAnimation(mIsRunning ? "WalkRight"
                                                           : "IdleRight");
        }
        case FaceDirection::South: {
            return mDrawComponent->SetAnimation(mIsRunning ? "WalkFront"
                                                           : "IdleFront");
        }
        case FaceDirection::North: {
            return mDrawComponent->SetAnimation(mIsRunning ? "WalkBack"
                                                           : "IdleBack");
        }
    }
}

void Player::OnHorizontalCollision(const float minOverlap,
                                   AABBColliderComponent *other) {
    // Later handle collision
}

void Player::OnVerticalCollision(const float minOverlap,
                                 AABBColliderComponent *other) {
    // Later handle collision
}

SDL_Scancode Player::GetDownCode() {
    switch (mPlayerType) {
        case PlayerType::PlayerB:
            return SDL_SCANCODE_S;
        case PlayerType::PlayerD:
            return SDL_SCANCODE_DOWN;
    }
    SDL_Log("Invalid player %d", (int)mPlayerType);
    return SDL_SCANCODE_0;
}

SDL_Scancode Player::GetLeftCode() {
    switch (mPlayerType) {
        case PlayerType::PlayerB:
            return SDL_SCANCODE_A;
        case PlayerType::PlayerD:
            return SDL_SCANCODE_LEFT;
    }
    SDL_Log("Invalid player %d", (int)mPlayerType);
    return SDL_SCANCODE_0;
}

SDL_Scancode Player::GetRightCode() {
    switch (mPlayerType) {
        case PlayerType::PlayerB:
            return SDL_SCANCODE_D;
        case PlayerType::PlayerD:
            return SDL_SCANCODE_RIGHT;
    }
    SDL_Log("Invalid player %d", (int)mPlayerType);
    return SDL_SCANCODE_0;
}

SDL_Scancode Player::GetUpCode() {
    switch (mPlayerType) {
        case PlayerType::PlayerB:
            return SDL_SCANCODE_W;
        case PlayerType::PlayerD:
            return SDL_SCANCODE_UP;
    }
    SDL_Log("Invalid player %d", (int)mPlayerType);
    return SDL_SCANCODE_0;
}

SDL_Scancode Player::GetPickUpCode() {
    switch (mPlayerType) {
        case PlayerType::PlayerB:
            return SDL_SCANCODE_LSHIFT;
        case PlayerType::PlayerD:
            return SDL_SCANCODE_RSHIFT;
    }
    SDL_Log("Invalid player %d", (int)mPlayerType);
    return SDL_SCANCODE_0;
}

SDL_Scancode Player::GetChopCode() {
    switch (mPlayerType) {
        case PlayerType::PlayerB:
            return SDL_SCANCODE_LCTRL;
        case PlayerType::PlayerD:
            return SDL_SCANCODE_RCTRL;
    }
    SDL_Log("Invalid player %d", (int)mPlayerType);
    return SDL_SCANCODE_0;
}

SDL_Scancode Player::GetDashCode() {
    switch (mPlayerType) {
        case PlayerType::PlayerB:
            return SDL_SCANCODE_LALT;
        case PlayerType::PlayerD:
            return SDL_SCANCODE_RALT;
    }
    SDL_Log("Invalid player %d", (int)mPlayerType);
    return SDL_SCANCODE_0;
}
