#include "Pot.h"

#include "../Components/DrawComponents/DrawSpriteComponent.h"

const std::string Pot::POT_EMPTY_PATH = "../Assets/Prototype/Pot.png";
const std::string Pot::POT_TOMATO_SOUP_PATH =
    "../Assets/Prototype/PotTomatoSoup.png";
const std::string Pot::POT_BURNT_PATH = "../Assets/Prototype/PotBurnt.png";

Pot::Pot(Game* game, const std::string& texturePath)
    : Item(game, texturePath, ItemType::Pot,
           /* width = */ 32,
           /* height = */ 32,
           /* drawOrder = */ 200),
      mItemCounter(0) {
    mItemInside = {};
}

// Public Constructor that handles choosing the textures
Pot* Pot::NewPot(Game* game) { return new Pot(game, POT_EMPTY_PATH); }

Item* Pot::PutItem(Item* item) {
    if (!item) return item;

    // If no item, just accept it
    if (!mItemInside) {
        switch (item->GetItemType()) {
            case ItemType::TomatoCut: {
                mItemInside = ItemType::TomatoCut;
                mItemCounter = 1;
                mCookTime = 0.0f;
                mIsCooked = mIsBurnt = false;
                item->SetState(ActorState::Destroy);
                SDL_Log("New item in the pot!");
                // TODO: Feedback on cook progress
                mDrawComponent->UpdateTexture(POT_TOMATO_SOUP_PATH);
                return nullptr;
            }
            // In case someone transfers soup
            case ItemType::TomatoSoup: {
                mItemInside = ItemType::TomatoSoup;
                mItemCounter = 3;
                mCookTime = COOK_TIME_MAX;
                mIsCooked = true;
                mIsBurnt = false;
                mDrawComponent->UpdateTexture(POT_TOMATO_SOUP_PATH);
                return nullptr;
            }
            // In case someone transfers burnt food
            case ItemType::TomatoBurn: {
                mItemInside = ItemType::TomatoBurn;
                mItemCounter = 3;
                mCookTime = BURN_TIME_MAX;
                mIsBurnt = mIsCooked = true;
                mDrawComponent->UpdateTexture(POT_BURNT_PATH);
                return nullptr;
            }
        }
        // Not a supported item
        return item;
    }

    // Only accept if it's the same type
    if (*mItemInside != item->GetItemType()) {
        SDL_Log("Have to refuse,since types are different");
        return item;
    }

    // Stacking more items
    switch (*mItemInside) {
        case ItemType::TomatoCut: {
            // At most 3
            if (mItemCounter >= 3) break;
            mItemCounter += 1;
            item->SetState(ActorState::Destroy);
            // TODO: Show to the user that there are `mItemCounter` items inside
            // the pot
            SDL_Log("Another item in the pot, now there are %d", mItemCounter);
            return nullptr;
        }
    }
    // Can't use it
    return item;
}

std::optional<ItemType> Pot::PickItem() {
    if (!mItemInside) return {};

    // Only *done* food is returned
    switch (*mItemInside) {
        case ItemType::TomatoSoup: {
            mItemInside = {};
            mDrawComponent->UpdateTexture(POT_EMPTY_PATH);
            return ItemType::TomatoSoup;
        }
    }
    // Not done or burnt.
    return {};
}

void Pot::Clear() { mItemInside = {}; }

void Pot::ReturnItem(ItemType item) {
    mItemInside = item;
    switch (item) {
        case ItemType::TomatoCut:
        case ItemType::TomatoSoup: {
            mDrawComponent->UpdateTexture(POT_TOMATO_SOUP_PATH);
            break;
        }
        case ItemType::TomatoBurn: {
            mDrawComponent->UpdateTexture(POT_BURNT_PATH);
        }
    }
}

void Pot::OnUpdate(float deltaTime) {
    if (!mItemInside) return;

    // Check if cooking is done!
    if (mCookTime >= mItemCounter * COOK_TIME_MAX && !mIsCooked) {
        switch (*mItemInside) {
            case ItemType::TomatoCut: {
                // Case of tomatoes: We need 3 to get a soup
                if (mItemCounter < 3) break;
                // Swap the TomatoCut to TomatoSoup
                mItemInside = ItemType::TomatoSoup;
                mIsCooked = true;
                break;
            }
        }
    }

    // Check if burning!
    if (mCookTime >= mItemCounter * BURN_TIME_MAX && !mIsBurnt) {
        switch (*mItemInside) {
            case ItemType::TomatoCut:
            case ItemType::TomatoSoup: {
                // Swap the TomatoSoup to TomatoBurn
                mItemInside = ItemType::TomatoBurn;
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