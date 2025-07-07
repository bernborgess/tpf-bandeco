#pragma once

#include "Block.h"

class Drainer : public Block {
   public:
    Drainer(Game* game, std::pair<int, int> gridPos)
        : Block(game, "../Assets/Prototype/DishDrainer.png", gridPos) {}

    void OnUpdate(float deltaTime) override;

    int mCleanPlateCount = 0;

    Item* PickItemOnTop() override;

    static const std::string DRAINER_PATH;
    static const std::string DRAINER_PLATE_PATH;
    static const std::string DRAINER_PLATES_PATH;
};