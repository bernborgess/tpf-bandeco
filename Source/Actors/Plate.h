#pragma once

#include <string>

#include "Actor.h"
#include "Item.h"

class Plate : public Item {
   public:
    static Plate* NewPlate(Game* game);
    Item* PutItem(Item* item);
    std::set<ItemType> PickItems();

   private:
    Plate(Game* game, const std::string& texturePath);
    std::set<ItemType> mItems;

    static const std::string PLATE_EMPTY_PATH;
    static const std::string PLATE_TOMATO_SOUP_PATH;
};
