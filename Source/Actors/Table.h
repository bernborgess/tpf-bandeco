#pragma once

#include <string>

#include "Block.h"
#include "Item.h"

class Table : public Block {
   public:
    Table(Game* game, const std::string& texturePath,
          std::pair<int, int> gridPos);

    Item* GetItemOnTop() { return mItemOnTop; }
    void SetItemOnTop(Item* item) { mItemOnTop = item; }

   private:
    Item* mItemOnTop;
};