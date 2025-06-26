#include "Item.h"

#include "../Components/DrawComponents/DrawSpriteComponent.h"

Item::Item(Game* game, const std::string& texturePath, ItemType itemType)
    : Actor(game), mItemType(itemType) {
    new DrawSpriteComponent(this, texturePath, 32, 32, 200);
}

// Public Constructor that handles choosing the textures
Item* Item::NewItem(Game* game, ItemType itemType) {
    // Const map to convert item type to texture paths
    const std::map<ItemType, const std::string> texturePathMap = {
        {ItemType::Tomato, "../Assets/Prototype/Tomato.png"},
        {ItemType::TomatoCut, "../Assets/Prototype/TomatoCut.png"},
        {ItemType::TomatoSoup, "../Assets/Prototype/TomatoSoup.png"}};

    auto it = texturePathMap.find(itemType);
    if (it != texturePathMap.end()) {
        return new Item(game, it->second, itemType);
    }
    return nullptr;
}
