#pragma once

#include <string>

#include "Block.h"
#include "Item.h"

class Trash : public Block {
   public:
    Trash(Game* game, std::pair<int, int> gridPos);

    Item* SetItemOnTop(Item* item) override;

   private:
    static const std::string TRASH_PATH;
};