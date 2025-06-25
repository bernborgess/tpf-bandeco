#include "FoodBoxBlock.h"

FoodBoxBlock::FoodBoxBlock(Game* game, const std::string& texturePath,
                           ItemType foodType, std::pair<int, int> gridPos)
    : Block(game, texturePath, gridPos), mFoodType(foodType) {}
