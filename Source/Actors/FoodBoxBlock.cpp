#include "FoodBoxBlock.h"

FoodBoxBlock::FoodBoxBlock(Game* game, const std::string& texturePath,
                           FoodType foodType)
    : Block(game, texturePath, true), mFoodType(foodType) {}
