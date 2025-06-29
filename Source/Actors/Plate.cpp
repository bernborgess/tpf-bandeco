#include "Plate.h"

#include "../Components/DrawComponents/DrawSpriteComponent.h"
#include "../Game.h"

// Paths to the plate variants
const std::string Plate::PLATE_EMPTY_PATH = "../Assets/Prototype/Plate.png";
const std::string Plate::PLATE_TOMATO_SOUP_PATH =
    "../Assets/Prototype/PlateTomatoSoup.png";

Plate::Plate(Game* game, const std::string& texturePath)
    : Item(game, texturePath, ItemType::Plate,
           /* width = */ 32,
           /* height = */ 32,
           /* drawOrder = */ 200) {
     mItems.clear();
}

// Public Constructor that handles choosing the textures
Plate* Plate::NewPlate(Game* game) { return new Plate(game, PLATE_EMPTY_PATH); }

Item* Plate::PutItem(Item* item) {
    if (!item) return item;

    switch (item->GetItemType()) {
        case ItemType::TomatoSoup: {
            // Accepted the item if plate is empty
            if (mItems.empty()) {
                // Change the item set
                mItems.insert(ItemType::TomatoSoup);

                // Destroy the tomato soup item
                item->SetState(ActorState::Destroy);

                // Update the DrawComponent
                mDrawComponent->UpdateTexture(PLATE_TOMATO_SOUP_PATH);
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
