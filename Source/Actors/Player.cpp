#include "Player.h"

#include "../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"
#include "../Game.h"
#include "Block.h"
#include "Item.h"
#include "Table.h"
#include "TableCut.h"

Player::Player(Game *game, const PlayerType playerType,
               const float forwardSpeed, const float jumpSpeed)
    : Actor(game),
      mIsRunning(false),
      mPlayerType(playerType),
      mForwardSpeed(forwardSpeed),
      mHandItem(nullptr) {
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

        // TODO: Remove this debug
        if (mHandItem == nullptr) {
            SDL_Log("Player %d got nothing", (int)mPlayerType);
        } else {
            SDL_Log("Player %d got %d", (int)mPlayerType,
                    (int)mHandItem->mItemType);
        }
    }
    if (scanCode == GetChopCode()) {
        HandleChop();
        SDL_Log("Player %d CHOP", (int)mPlayerType);
    }
    if (scanCode == GetDashCode()) {
        SDL_Log("Player %d DASH", (int)mPlayerType);
    }
}

std::tuple<LevelDataEntry, int, int> Player::GetFocusBlock() {
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
    LevelDataEntry levelEntry = mGame->mLevelData[pyg][pxg];
    SDL_Log("Level entry is a %d", (int)levelEntry);
    return {levelEntry, pxg, pyg};
}

// Assumes player has empty hand
void Player::HandlePickUp() {
    if (mHandItem != nullptr) return;
    auto [levelEntry, pxg, pyg] = GetFocusBlock();

    // Getting food from box
    if (levelEntry == LevelDataEntry::TileFoodTomato) {
        const std::string tomatoTilePath = "../Assets/Prototype/Tomato.png";
        mHandItem = new Item(mGame, tomatoTilePath, ItemType::Tomato);
    } else if (levelEntry == LevelDataEntry::TileTable) {
        Block *block = mGame->GetBlockAt(pxg, pyg);
        if (block == nullptr) {
            SDL_Log("Expected a table, didn't find it!");
            return;
        }
        // I know it's a table
        Table *table = (Table *)block;
        Item *item = table->GetItemOnTop();
        if (item) {
            // Remove item from if hand is empty
            mHandItem = item;
            table->SetItemOnTop(nullptr);
        }
    } else if (levelEntry == LevelDataEntry::TileTableCut) {
        Block *block = mGame->GetBlockAt(pxg, pyg);
        if (block == nullptr) {
            SDL_Log("Expected a tableCut, didn't find it!");
            return;
        }
        // I know it's a table
        TableCut *tableCut = (TableCut *)block;
        Item *item = tableCut->GetItemOnTop();
        if (item) {  // get item from table

            // TODO: Only allow removing if cut level is either 0 or
            // CUT_LEVEL_MAX
            mHandItem = item;
            tableCut->SetItemOnTop(nullptr);
            // If it's CUT_LEVEL_MAX, you should return the cut version of the
            // item.
        }
    }
}

// Assumes player has item in hand
void Player::HandlePutDown() {
    if (mHandItem == nullptr) return;
    const auto [levelEntry, pxg, pyg] = GetFocusBlock();

    if (levelEntry == LevelDataEntry::TileTable) {
        Block *block = mGame->GetBlockAt(pxg, pyg);
        if (block == nullptr) {
            SDL_Log("Expected a table, didn't find it!");
            return;
        }
        // I know it's a table
        Table *table = (Table *)block;
        Item *item = table->GetItemOnTop();
        if (!item) {  // Put the item on the empty table
            table->SetItemOnTop(mHandItem);
            mHandItem->SetPosition(table->GetPosition() + Vector2(16, 8));
            mHandItem = nullptr;
        }
    } else if (levelEntry == LevelDataEntry::TileTableCut) {
        Block *block = mGame->GetBlockAt(pxg, pyg);
        if (block == nullptr) {
            SDL_Log("Expected a tableCut, didn't find it!");
            return;
        }
        // I know it's a table
        Table *table = (Table *)block;
        Item *item = table->GetItemOnTop();
        if (!item) {
            // Put the item on the empty table
            table->SetItemOnTop(mHandItem);
            mHandItem->SetPosition(table->GetPosition() + Vector2(16, 8));
            mHandItem = nullptr;
        }
    }
}

// If it's a TableCut and there's food on it, will chop
void Player::HandleChop() {
    if (mHandItem != nullptr) return;
    const auto [levelEntry, pxg, pyg] = GetFocusBlock();
    if (levelEntry != LevelDataEntry::TileTableCut) return;
    Block *block = mGame->GetBlockAt(pxg, pyg);
    if (block == nullptr) {
        SDL_Log("Expected a tableCut, didn't find it!");
        return;
    }
    TableCut *tableCut = (TableCut *)block;
    Item *item = tableCut->GetItemOnTop();
    if (!item) return;  // empty table
    // TODO: Increase cut level if it's not maxed out
    if (item->mItemType == ItemType::Tomato) {
        const std::string cutTomatoTilePath =
            "../Assets/Prototype/TomatoCut.png";
        Item *cutTomato =
            new Item(mGame, cutTomatoTilePath, ItemType::CutTomato);
        delete item;
        tableCut->SetItemOnTop(cutTomato);
        cutTomato->SetPosition(tableCut->GetPosition() + Vector2(16, 8));
    }
}

void Player::OnUpdate(float deltaTime) {
    // TODO: Check win and set the game scene to Level 2
    // mState = ActorState::Destroy;
    // mGame->SetGameScene(Game::GameScene::Level2, 3.5f);

    // Hand Item follows player
    if (mHandItem != nullptr) {
        mHandItem->SetPosition(mPosition);
    }

    ManageAnimations();
}

void Player::ManageAnimations() {
    if (mIsRunning) {
        mDrawComponent->SetAnimation("run");
    } else {
        mDrawComponent->SetAnimation("idle");
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
