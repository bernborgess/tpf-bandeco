#pragma once

#include <string>

#include "Block.h"
#include "Pot.h"

class Stove : public Block {
   public:
    Stove(Game* game, const std::string& texturePath,
          std::pair<int, int> gridPos);

    Item* PickItemOnTop() override;
    Item* SetItemOnTop(Item* item) override;

    void OnUpdate(float deltaTime) override;

   protected:
    Pot* mPotOnTop;
};