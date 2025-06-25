#pragma once

#include <string>

#include "Block.h"
#include "Item.h"

class FoodBoxBlock : public Block {
   public:
    FoodBoxBlock(Game* game, const std::string& texturePath, ItemType foodType);

    ItemType mFoodType;
};