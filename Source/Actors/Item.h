#pragma once

#include <string>

#include "Actor.h"

enum class ItemType { Plate, Tomato, CutTomato, TomatoSoup };

class Item : public Actor {
   public:
    Item(Game* game, const std::string& texturePath, ItemType itemType);

    bool mIsHoldable;
    ItemType mItemType;
};