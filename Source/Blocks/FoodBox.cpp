#include "FoodBox.h"

const std::string FoodBox::FOOD_BOX_BREAD_PATH =
    "../Assets/Prototype/FoodBread.png";
const std::string FoodBox::FOOD_BOX_LETTUCE_PATH =
    "../Assets/Prototype/FoodLettuce.png";
const std::string FoodBox::FOOD_BOX_MEAT_PATH =
    "../Assets/Prototype/FoodMeat.png";
const std::string FoodBox::FOOD_BOX_TOMATO_PATH =
    "../Assets/Prototype/FoodTomato.png";

FoodBox::FoodBox(Game* game, const std::string& texturePath, ItemType foodType,
                 std::pair<int, int> gridPos)
    : Block(game, texturePath, gridPos), mFoodType(foodType) {}

FoodBox* FoodBox::NewFoodBox(Game* game, ItemType foodType,
                             std::pair<int, int> gridPos) {
    switch (foodType) {
        case ItemType::Tomato: {
            return new FoodBox(game, FOOD_BOX_TOMATO_PATH, foodType, gridPos);
        }
        case ItemType::Lettuce: {
            return new FoodBox(game, FOOD_BOX_LETTUCE_PATH, foodType, gridPos);
        }
    }
    return nullptr;
}

Item* FoodBox::PickItemOnTop() { return Item::NewItem(mGame, mFoodType); }