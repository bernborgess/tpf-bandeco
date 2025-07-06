#pragma once

#include <string>

#include "../Actors/ProgressBar.h"
#include "Table.h"

class TableCut : public Table {
   public:
    static TableCut* NewTableCut(Game*, LevelTile tile,
                                 std::pair<int, int> gridPos);

    // Changes item if finished cutting
    Item* PickItemOnTop() override;

    Item* SetItemOnTop(Item* item) override;

    void OnItemCut();

   private:
    TableCut(Game* game, const std::string& texturePath,
             std::pair<int, int> gridPos);

    static const int CUT_LEVEL_MAX = 5;
    int cutLevel;

    static const Vector2 TOMATO_OFFSET;

    static const std::string TABLE_CUT_FRONT_PATH;
    static const std::string TABLE_CUT_RIGHT_PATH;

    ProgressBar* mProgressBar;
};