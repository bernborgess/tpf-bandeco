#pragma once

#include <string>

#include "Table.h"

class Stove : public Table {
   public:
    Stove(Game* game, const std::string& texturePath,
          std::pair<int, int> gridPos);

    // TODO: Tick cooking time

   private:
    static const int CUT_LEVEL_MAX = 5;
    int cutLevel;
};