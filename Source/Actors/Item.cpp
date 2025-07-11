#include "Item.h"

#include "../Components/DrawComponents/DrawSpriteComponent.h"

Item::Item(Game* game, const std::string& texturePath, ItemType itemType,
           int width /* = 32 */, int height /* = 32 */,
           int drawOrder /* = 300 */)
    : Actor(game), mItemType(itemType) {
    mDrawComponent =
        new DrawSpriteComponent(this, texturePath, width, height, drawOrder);
}

// Public Constructor that handles choosing the textures
Item* Item::NewItem(Game* game, ItemType itemType) {
    // Const map to convert item type to texture paths
    const std::map<ItemType, const std::string> texturePathMap = {
        {ItemType::Tomato, "../Assets/Prototype/Tomato.png"},
        {ItemType::TomatoCut, "../Assets/Prototype/TomatoCut.png"},
        {ItemType::Lettuce, "../Assets/Prototype/Lettuce.png"},
        {ItemType::LettuceCut, "../Assets/Prototype/LettuceCut.png"},
        {ItemType::Bread, "../Assets/Prototype/Bread.png"},
        {ItemType::Meat, "../Assets/Prototype/Meat.png"},
        {ItemType::MeatCut, "../Assets/Prototype/MeatCut.png"},
        {ItemType::PlateDirty, "../Assets/Sprites/Plates/Dirty.png"},
    };

    auto it = texturePathMap.find(itemType);
    if (it != texturePathMap.end()) {
        return new Item(game, it->second, itemType);
    }
    return nullptr;
}

void Item::ResetAbove() { mDrawComponent->ResetAbove(); }

void Item::SetAbove() { mDrawComponent->SetAbove(); }
