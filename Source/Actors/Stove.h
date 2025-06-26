#pragma once

#include <string>

#include "Block.h"
#include "Pot.h"

class Stove : public Block {
   public:
    Stove(Game* game, const std::string& texturePath,
          std::pair<int, int> gridPos);

    bool HasPotOnTop() { return mPotOnTop != nullptr; }

    // TODO: Tick cooking time

   protected:
    Pot* mPotOnTop;
};