#pragma once

#include <string>

#include "Block.h"
#include "Item.h"
#include "Plate.h"

class Deliver : public Block {
   public:
    Deliver(Game* game, const std::string& texturePath,
            std::pair<int, int> gridPos)
        : Block(game, texturePath, gridPos) {}

    Item* SetItemOnTop(Item* item);
};