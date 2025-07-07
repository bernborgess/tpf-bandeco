#pragma once

#include "Block.h"

class Cabinet : public Block {
   public:
    Cabinet(Game* game, std::pair<int, int> gridPos)
        : Block(game, "../Assets/Prototype/Cabinet.png", gridPos) {}

    void OnUpdate(float deltaTime) override;

    int mDirtyPlateCount = 0;

    Item* PickItemOnTop() override;

   private:
    static const std::string CABINET_EMPTY_PATH;
    static const std::string CABINET_PLATE_PATH;
    static const std::string CABINET_PLATES_PATH;
};