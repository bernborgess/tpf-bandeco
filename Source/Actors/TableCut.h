#pragma once

#include <string>

#include "Table.h"

class TableCut : public Table {
   public:
    TableCut(Game* game, const std::string& texturePath,
             std::pair<int, int> gridPos);

    // Changes item if finished cutting
    Item* PickItemOnTop();

    Item* SetItemOnTop(Item* item);

    void OnItemCut();

   private:
    static const int CUT_LEVEL_MAX = 5;
    int cutLevel;
};