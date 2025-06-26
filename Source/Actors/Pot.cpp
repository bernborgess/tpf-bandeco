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
    // If no item, just accept it
    if (!mItemInside) {
        SDL_Log("No item in pot, just accept");
        mItemInside = item;
        mItemCounter = 1;
        return nullptr;
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

void Pot::OnUpdate(float deltaTime) {
    if (mItemInside) {
        mItemInside->SetPosition(GetPosition() + Vector2(0, -10));
    }
}