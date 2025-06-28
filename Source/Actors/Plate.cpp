#include "Plate.h"

#include "../Components/DrawComponents/DrawSpriteComponent.h"
#include "../Game.h"

Plate::Plate(Game* game, const std::string& texturePath)
    : Item(game, texturePath, ItemType::Plate, 200) {
    mItems.clear();
}

// Public Constructor that handles choosing the textures
Plate* Plate::NewPlate(Game* game) {
    return new Plate(game, "../Assets/Prototype/Plate.png");
}

Item* Plate::PutItem(Item* item) {
    if (!item) return item;

    switch (item->GetItemType()) {
        case ItemType::TomatoSoup: {
            // Accepted the item if plate is empty
            if (mItems.empty()) {
                // TODO: Update the DrawComponent
                mItems.insert(ItemType::TomatoSoup);
                item->SetState(ActorState::Destroy);
                return nullptr;
            }
        }
            // Other food will accept on different rules
    }

    // Can't use it, reject
    return item;
}

std::set<ItemType> Plate::PickItems() {
    std::set<ItemType> items = mItems;
    mItems.clear();
    return items;
}
