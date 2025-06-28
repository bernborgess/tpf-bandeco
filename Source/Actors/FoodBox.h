#pragma once

#include <string>

#include "Block.h"
#include "Item.h"

class FoodBox : public Block {
   public:
    FoodBox(Game* game, const std::string& texturePath, ItemType foodType,
            std::pair<int, int> gridPos);

    Item* PickItemOnTop() override;

    ItemType mFoodType;
};