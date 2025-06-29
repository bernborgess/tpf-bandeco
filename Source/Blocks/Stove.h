#pragma once

#include <string>

#include "../Actors/Pot.h"
#include "../Level.h"
#include "Block.h"

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