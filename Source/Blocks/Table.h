#pragma once

#include <string>

#include "../Actors/Item.h"
#include "../Level.h"
#include "Block.h"

class Table : public Block {
   public:
    static Table* NewTable(Game* game, LevelTile tile,
                           std::pair<int, int> gridPos);

    // Returns the item and empties the table
    Item* PickItemOnTop() override;

    // Peek without changes
    bool HasItemOnTop();

    // Returns nullptr if it got the item, returns the item if rejected
    Item* SetItemOnTop(Item* item) override;

   protected:
    Table(Game* game, const std::string& texturePath,
          std::pair<int, int> gridPos);

    Item* mItemOnTop;

    static const std::string TABLE_CENTER_PATH;
    static const std::string TABLE_FRONT_PATH;
    static const std::string TABLE_LEFT_PATH;
    static const std::string TABLE_PLAIN_PATH;
    static const std::string TABLE_RIGHT_PATH;
};