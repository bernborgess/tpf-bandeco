#pragma once

#include <string>

#include "Actor.h"

enum class ItemType { Plate, Tomato, TomatoCut, TomatoSoup };

class Item : public Actor {
   public:
    static Item* NewItem(Game* game, ItemType itemType);
    ItemType GetItemType() { return mItemType; }

   private:
    Item(Game* game, const std::string& texturePath, ItemType itemType);
    bool mIsHoldable;
    ItemType mItemType;
};