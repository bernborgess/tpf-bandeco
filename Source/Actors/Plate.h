#pragma once

#include <optional>
#include <string>

#include "Actor.h"
#include "Item.h"

class Plate : public Item {
   public:
    static Plate* NewPlate(Game* game);

    // Adds item to the plate, return the item if rejected
    Item* PutItem(Item* item);

    // Add itemType to the plate, returns the item if rejected
    std::optional<ItemType> PutItem(ItemType itemType);

    // Returns the item set, empties itself
    std::set<ItemType> PickItems();

   private:
    Plate(Game* game, const std::string& texturePath);
    std::set<ItemType> mItems;

    static const std::string PLATE_EMPTY_PATH;
    static const std::string PLATE_TOMATO_SOUP_PATH;
    static const std::string PLATE_TOMATO_CUT_PATH;
};
