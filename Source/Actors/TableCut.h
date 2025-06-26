#pragma once

#include <string>

#include "Table.h"

class TableCut : public Table {
   public:
    TableCut(Game* game, const std::string& texturePath,
             std::pair<int, int> gridPos);

    void OnItemCut();
};