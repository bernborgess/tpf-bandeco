#include "Item.h"

#include "../Components/DrawComponents/DrawSpriteComponent.h"

Item::Item(Game* game, const std::string& texturePath, ItemType itemType)
    : Actor(game), mItemType(itemType) {
    new DrawSpriteComponent(this, texturePath, 32, 32, 200);
}