#include "FoodBox.h"

FoodBox::FoodBox(Game* game, const std::string& texturePath, ItemType foodType,
                 std::pair<int, int> gridPos)
    : Block(game, texturePath, gridPos), mFoodType(foodType) {}

Item* FoodBox::PickItemOnTop() { return Item::NewItem(mGame, mFoodType); }