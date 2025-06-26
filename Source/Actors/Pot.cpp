#include "Pot.h"

#include "../Components/DrawComponents/DrawSpriteComponent.h"

Pot::Pot(Game* game, const std::string& texturePath)
    : Item(game, texturePath, ItemType::Pot),
      mItemCounter(0),
      mItemInside(nullptr) {
    new DrawSpriteComponent(this, texturePath, 32, 32, 150);
}

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

    mItemCounter += 1;
    delete item;
    SDL_Log("Another item in the pot, now there are %d", mItemCounter);
    return nullptr;
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
    if (mCookTime >= COOK_TIME_MAX && !isCooked) {
        // TODO: Swap the TomatoCut to TomatoSoup
        SDL_Log("COOKED!");
        isCooked = true;
    }
}

void Pot::OnCook(float deltaTime) {
    if (mItemInside) {
        mCookTime += deltaTime;
    }
}