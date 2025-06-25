#include "Table.h"

Table::Table(Game* game, const std::string& texturePath,
             std::pair<int, int> gridPos)
    : Block(game, texturePath, gridPos), mItemOnTop(nullptr) {}
