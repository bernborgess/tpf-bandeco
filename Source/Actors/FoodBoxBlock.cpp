#include "FoodBoxBlock.h"

FoodBoxBlock::FoodBoxBlock(Game* game, const std::string& texturePath,
                           ItemType foodType)
    : Block(game, texturePath, true), mFoodType(foodType) {}
