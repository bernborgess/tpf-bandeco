#pragma once

#include <string>

#include "Block.h"
#include "Pot.h"

class Stove : public Block {
   public:
    Stove(Game* game, const std::string& texturePath,
          std::pair<int, int> gridPos);

    bool HasPotOnTop() { return mPotOnTop != nullptr; }
    Pot* PickPotOnTop();
    Pot* PutPotOnTop(Pot* pot);
    Item* PutFoodInPot(Item* item);

    void OnUpdate(float deltaTime) override;

   protected:
    Pot* mPotOnTop;
};