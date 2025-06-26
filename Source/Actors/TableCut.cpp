#include "TableCut.h"

#include <SDL.h>

TableCut::TableCut(Game* game, const std::string& texturePath,
                   std::pair<int, int> gridPos)
    : Table(game, texturePath, gridPos) {
    SDL_Log("New TableCut Created");
}

void TableCut::OnItemCut() { SDL_Log("HEY!"); }