#include "Pot.h"

#include "../Components/DrawComponents/DrawSpriteComponent.h"

Pot::Pot(Game* game, const std::string& texturePath)
    : Item(game, texturePath, ItemType::Pot, 200),
      mItemCounter(0),
      mItemInside(nullptr) {}

// Public Constructor that handles choosing the textures
Pot* Pot::NewPot(Game* game) {
    return new Pot(game, "../Assets/Prototype/Pot.png");
}

Item* Pot::PutItem(Item* item) {
    if (!item) return item;

    // If no item, just accept it
    if (!mItemInside) {
        switch (item->GetItemType()) {
            case ItemType::TomatoCut: {
                mItemInside = item;
                mItemCounter = 1;
                mCookTime = 0.0f;
                mIsCooked = mIsBurnt = false;
                SDL_Log("New item in the pot!");
                return nullptr;
            }
            // In case someone transfers soup
            case ItemType::TomatoSoup: {
                mItemInside = item;
                mItemCounter = 3;
                mCookTime = COOK_TIME_MAX;
                mIsCooked = true;
                mIsBurnt = false;
                return nullptr;
            }
            // In case someone transfers burnt food
            case ItemType::TomatoBurn: {
                mItemInside = item;
                mItemCounter = 3;
                mCookTime = BURN_TIME_MAX;
                mIsBurnt = mIsCooked = true;
                return nullptr;
            }
        }
        // Not a supported item
        return item;
    }
    // Only accept if it's the same type
    if (mItemInside->GetItemType() != item->GetItemType()) {
        SDL_Log("Have to refuse,since types are different");
        return item;
    }

    // Stacking more items
    switch (mItemInside->GetItemType()) {
        case ItemType::TomatoCut: {
            // At most 3
            if (mItemCounter >= 3) break;
            mItemCounter += 1;
            item->SetState(ActorState::Destroy);
            SDL_Log("Another item in the pot, now there are %d", mItemCounter);
            return nullptr;
        }
    }
    // Can't use it
    return item;
}

Item* Pot::PickItem() {
    if (!mItemInside) return nullptr;

    // TODO: Resolve when cooking is done
    Item* item = mItemInside;
    mItemInside = nullptr;
    return item;
}

void Pot::OnUpdate(float deltaTime) {
    if (!mItemInside) return;
    mItemInside->SetPosition(GetPosition() + Vector2(0, -10));

    // Check if cooking is done!
    if (mCookTime >= mItemCounter * COOK_TIME_MAX && !mIsCooked) {
        switch (mItemInside->GetItemType()) {
            case ItemType::TomatoCut: {
                // Case of tomatoes: We need 3 to get a soup
                if (mItemCounter < 3) break;
                // Swap the TomatoCut to TomatoSoup
                Item* soup = NewItem(mGame, ItemType::TomatoSoup);
                mItemInside->SetState(ActorState::Destroy);
                mItemInside = soup;
                mIsCooked = true;
                break;
            }
        }
    }

    // Check if burning!
    if (mCookTime >= mItemCounter * BURN_TIME_MAX && !mIsBurnt) {
        switch (mItemInside->GetItemType()) {
            case ItemType::TomatoCut:
            case ItemType::TomatoSoup: {
                // Swap the TomatoSoup to TomatoBurn
                Item* burnt = NewItem(mGame, ItemType::TomatoBurn);
                mItemInside->SetState(ActorState::Destroy);
                mItemInside = burnt;
                mIsBurnt = true;
                break;
            }
        }
    }
}

void Pot::OnCook(float deltaTime) {
    if (mItemInside) {
        mCookTime += deltaTime;
    }
}