#pragma once

#include <string>

#include "../Actors/Item.h"
#include "Block.h"

class Trash : public Block {
   public:
    Trash(Game* game, std::pair<int, int> gridPos);

    Item* SetItemOnTop(Item* item) override;

   private:
    static const std::string TRASH_PATH;
};