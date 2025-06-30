#include "Pot.h"

#include "../Components/DrawComponents/DrawSpriteComponent.h"
#include "Plate.h"

const std::string Pot::POT_EMPTY_PATH = "../Assets/Prototype/Pot.png";
const std::string Pot::POT_TOMATO_1_PATH = "../Assets/Prototype/PotTomato1.png";
const std::string Pot::POT_TOMATO_2_PATH = "../Assets/Prototype/PotTomato2.png";
const std::string Pot::POT_TOMATO_3_PATH = "../Assets/Prototype/PotTomato3.png";
const std::string Pot::POT_TOMATO_SOUP_PATH =
    "../Assets/Prototype/PotTomatoSoup.png";
const std::string Pot::POT_BURNT_PATH = "../Assets/Prototype/PotBurnt.png";

Pot::Pot(Game* game, const std::string& texturePath)
    : Item(game, texturePath, ItemType::Pot,
           /* width = */ 64,
           /* height = */ 64,
           /* drawOrder = */ 200),
      mItemCounter(0) {
    mItemInside = {};
}

// Public Constructor that handles choosing the textures
Pot* Pot::NewPot(Game* game) { return new Pot(game, POT_EMPTY_PATH); }

bool Pot::AddItem(ItemType itemType) {
    if (!mItemInside) {
        switch (itemType) {
            case ItemType::TomatoCut: {
                mItemInside = ItemType::TomatoCut;
                mItemCounter = 1;
                mCookTime = 0.0f;
                mIsCooked = mIsBurnt = false;
                mDrawComponent->UpdateTexture(POT_TOMATO_1_PATH);
                return true;
            }
            // In case someone transfers soup
            case ItemType::TomatoSoup: {
                mItemInside = ItemType::TomatoSoup;
                mItemCounter = 3;
                mCookTime = COOK_TIME_MAX;
                mIsCooked = true;
                mIsBurnt = false;
                mDrawComponent->UpdateTexture(POT_TOMATO_SOUP_PATH);
                return true;
            }
            // In case someone transfers burnt food
            case ItemType::TomatoBurn: {
                mItemInside = ItemType::TomatoBurn;
                mItemCounter = 3;
                mCookTime = BURN_TIME_MAX;
                mIsBurnt = mIsCooked = true;
                mDrawComponent->UpdateTexture(POT_BURNT_PATH);
                return true;
            }
        }
        // Not a supported item
        return false;
    }

    // Only accept if it's the same type
    if (*mItemInside != itemType) {
        SDL_Log("Have to refuse,since types are different");
        return false;
    }

    // Stacking more items
    switch (*mItemInside) {
        case ItemType::TomatoCut: {
            // At most 3
            if (mItemCounter >= 3) break;
            mItemCounter += 1;
            if (mItemCounter == 1)
                mDrawComponent->UpdateTexture(POT_TOMATO_1_PATH);
            if (mItemCounter == 2)
                mDrawComponent->UpdateTexture(POT_TOMATO_2_PATH);
            if (mItemCounter == 3)
                mDrawComponent->UpdateTexture(POT_TOMATO_3_PATH);
            return true;
        }
    }
    // Can't use it
    return false;
}

Item* Pot::PutItem(Item* item) {
    if (!item) return item;

    bool accepted = AddItem(item->GetItemType());

    if (accepted) {
        item->SetState(ActorState::Destroy);
        return nullptr;
    } else if (item->GetItemType() == ItemType::Plate) {
        SDL_Log("Adding plate to pot? Just pass the ingredients");
        Plate* plate = (Plate*)item;
        auto items = plate->PickItems();
        for (auto& itemType : items) {
            bool accepted = AddItem(itemType);
            if (!accepted) {
                // Give back to the plate
                plate->PutItem(itemType);
            }
        }
        return plate;
    } else {
        return item;
    }
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
        case ItemType::TomatoCut: {
            if (mItemCounter == 1)
                mDrawComponent->UpdateTexture(POT_TOMATO_1_PATH);
            if (mItemCounter == 2)
                mDrawComponent->UpdateTexture(POT_TOMATO_2_PATH);
            if (mItemCounter == 3)
                mDrawComponent->UpdateTexture(POT_TOMATO_3_PATH);
        }
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
                mDrawComponent->UpdateTexture(POT_TOMATO_SOUP_PATH);
                SDL_Log("SOUP IS COOKED!");
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
                mDrawComponent->UpdateTexture(POT_BURNT_PATH);
                SDL_Log("SOUP JUST BURNT!");
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