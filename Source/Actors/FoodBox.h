#pragma once

#include <string>

#include "Block.h"
#include "Item.h"

class FoodBox : public Block {
   public:
    static FoodBox* NewFoodBox(Game* game, ItemType foodType,
                               std::pair<int, int> gridPos);

    Item* PickItemOnTop() override;

    ItemType mFoodType;

   private:
    FoodBox(Game* game, const std::string& texturePath, ItemType foodType,
            std::pair<int, int> gridPos);
    static const std::string FOOD_BOX_BREAD_PATH;
    static const std::string FOOD_BOX_LETTUCE_PATH;
    static const std::string FOOD_BOX_MEAT_PATH;
    static const std::string FOOD_BOX_TOMATO_PATH;
};