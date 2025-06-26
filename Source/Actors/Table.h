#pragma once

#include <string>

#include "Block.h"
#include "Item.h"

class Table : public Block {
   public:
    Table(Game* game, const std::string& texturePath,
          std::pair<int, int> gridPos);

    // Returns the item and empties the table
    Item* PickItemOnTop();

    // Peek without changes
    bool HasItemOnTop();

    // Returns nullptr if it got the item, returns the item if rejected
    Item* SetItemOnTop(Item* item);

   protected:
    Item* mItemOnTop;
};