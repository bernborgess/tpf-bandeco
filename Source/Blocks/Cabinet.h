#pragma once

#include "Block.h"

class Cabinet : public Block {
   public:
    Cabinet(Game* game, std::pair<int, int> gridPos)
        : Block(game, "../Assets/Prototype/Cabinet.png", gridPos) {}

    int mDirtyPlateCount = 0;

    Item* PickItemOnTop() override;
};