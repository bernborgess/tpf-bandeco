#include "Plate.h"

#include "../Components/DrawComponents/DrawSpriteComponent.h"

Plate::Plate(Game* game, const std::string& texturePath)
    : Item(game, texturePath, ItemType::Plate) {
    mItems.clear();
    new DrawSpriteComponent(this, texturePath, 32, 32, 150);
}

// Public Constructor that handles choosing the textures
Plate* Plate::NewPlate(Game* game) {
    return new Plate(game, "../Assets/Prototype/Plate.png");
}

Item* Plate::PutItem(Item* item) {
    if (!item) return item;

    // TODO
}

void Plate::Deliver() {
    // TODO: Interact with the Deliver block and check if there's
    // this recibe on the orders queue
}