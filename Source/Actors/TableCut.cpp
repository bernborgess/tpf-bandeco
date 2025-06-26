#include "TableCut.h"

#include <SDL.h>

TableCut::TableCut(Game* game, const std::string& texturePath,
                   std::pair<int, int> gridPos)
    : Table(game, texturePath, gridPos) {}

Item* TableCut::PickItemOnTop() {
    if (!mItemOnTop) return nullptr;

    // Didn't finish the cut
    if (cutLevel > 0 && cutLevel < CUT_LEVEL_MAX) return nullptr;

    Item* item = mItemOnTop;
    mItemOnTop = nullptr;
    return item;
}

Item* TableCut::SetItemOnTop(Item* item) {
    if (mItemOnTop) {
        // Rejected the item (table full)
        SDL_Log("Rejected the item (table full)");
        return item;
    }

    // TODO: Check item is cuttable
    mItemOnTop = item;
    mItemOnTop->SetPosition(GetPosition() + Vector2(16, 8));
    cutLevel = 0;
    return nullptr;
}

void TableCut::OnItemCut() {
    if (mItemOnTop == nullptr) return;
    if (cutLevel == CUT_LEVEL_MAX) return;  // Finished
    cutLevel++;                             // Add a cut
    SDL_Log("CUT lvl %d!", cutLevel);
    if (cutLevel == CUT_LEVEL_MAX) {
        SDL_Log("Finished cut!");

        // Now transforms into finished item
        if (mItemOnTop->GetItemType() == ItemType::Tomato) {
            Item* cutTomato = Item::NewItem(mGame, ItemType::TomatoCut);
            delete mItemOnTop;
            mItemOnTop = cutTomato;
            cutTomato->SetPosition(GetPosition() + Vector2(16, 8));
        }
    }
}