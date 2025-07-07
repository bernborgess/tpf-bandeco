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

    // Checks for content
    bool HasFood() { return mItems.size() != 0; }

   private:
    Plate(Game* game, const std::string& texturePath);
    std::set<ItemType> mItems;

    void UpdateTexture();

    static const std::string PLATE_EMPTY_PATH;
    static const std::string PLATE_TOMATO_SOUP_PATH;
    static const std::string PLATE_TOMATO_CUT_PATH;

    // This sandwich is a combinatorial nightmare of (Bread Lettuce Meat Tomato)
    static const std::string PLATE_T_PATH;
    static const std::string PLATE_M_PATH;
    static const std::string PLATE_MT_PATH;
    static const std::string PLATE_L_PATH;
    static const std::string PLATE_LT_PATH;
    static const std::string PLATE_LM_PATH;
    static const std::string PLATE_LMT_PATH;
    static const std::string PLATE_B_PATH;
    static const std::string PLATE_BT_PATH;
    static const std::string PLATE_BM_PATH;
    static const std::string PLATE_BMT_PATH;
    static const std::string PLATE_BL_PATH;
    static const std::string PLATE_BLT_PATH;
    static const std::string PLATE_BLM_PATH;
    static const std::string PLATE_BLMT_PATH;
};
