#include "Plate.h"

#include "../Components/DrawComponents/DrawSpriteComponent.h"

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
                mItems.push_back(item);
                return nullptr;
            }
        }
            // Other food will accept on different rules
    }

    // Can't use it, reject
    return item;
}

void Plate::Deliver() {
    // TODO: Interact with the Deliver block and check if there's
    // this recibe on the orders queue
}

void Plate::OnUpdate(float deltaTime) {
    if (mItems.empty()) return;

    for (int i = 0; i < mItems.size(); i++) {
        Item*& item = mItems[i];
        item->SetPosition(GetPosition() + Vector2(0, -16 + 4 * i));
    }
}