#pragma once

#include <string>

#include "Actor.h"

enum class ItemType { Plate, Tomato, TomatoCut, TomatoSoup, TomatoBurn, Pot };

class Item : public Actor {
   public:
    static Item* NewItem(Game* game, ItemType itemType);
    ItemType GetItemType() { return mItemType; }

   protected:
    Item(Game* game, const std::string& texturePath, ItemType itemType,
         int width = 32, int height = 32, int drawOrder = 300);
    bool mIsHoldable;
    ItemType mItemType;

    class DrawSpriteComponent* mDrawComponent;
};