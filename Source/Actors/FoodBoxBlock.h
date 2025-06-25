#pragma once

#include <string>

#include "Block.h"

enum class FoodType { Tomato };

class FoodBoxBlock : public Block {
   public:
    FoodBoxBlock(Game* game, const std::string& texturePath, FoodType foodType);

    FoodType mFoodType;
};