#pragma once

#include <string>

#include "../Actors/Item.h"
#include "../Actors/Plate.h"
#include "../Level.h"
#include "Block.h"

class Deliver : public Block {
   public:
    static Deliver* NewDeliver(Game* game, LevelTile tile,
                               std::pair<int, int> gridPos);

    Item* SetItemOnTop(Item* item) override;

   private:
    Deliver(Game* game, const std::string& texturePath,
            std::pair<int, int> gridPos)
        : Block(game, texturePath, gridPos) {}

    static const std::string DELIVER_FRONT_PATH;
};