#pragma once

#include <string>

#include "../Level.h"
#include "Block.h"
#include "Pot.h"

class Stove : public Block {
   public:
    static Stove* NewStove(Game* game, LevelTile tile,
                           std::pair<int, int> gridPos);

    Item* PickItemOnTop() override;
    Item* SetItemOnTop(Item* item) override;

    void OnUpdate(float deltaTime) override;

   protected:
    Stove(Game* game, const std::string& texturePath,
          std::pair<int, int> gridPos);

    Pot* mPotOnTop;

    static const std::string STOVE_FRONT_PATH;
    static const std::string STOVE_RIGHT_PATH;
};