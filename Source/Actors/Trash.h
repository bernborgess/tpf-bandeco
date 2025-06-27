#pragma once

#include <string>

#include "Block.h"
#include "Item.h"

class Trash : public Block {
   public:
    Trash(Game* game, const std::string& texturePath,
          std::pair<int, int> gridPos)
        : Block(game, texturePath, gridPos) {}

    Item* SetItemOnTop(Item* item) override;
};