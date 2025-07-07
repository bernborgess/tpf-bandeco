#pragma once

#include "Block.h"

class Drainer : public Block {
   public:
    Drainer(Game* game, std::pair<int, int> gridPos)
        : Block(game, "../Assets/Prototype/DishDrainer.png", gridPos) {}

    int mCleanPlateCount = 0;

    Item* PickItemOnTop() override;
};