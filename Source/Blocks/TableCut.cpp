#include "TableCut.h"

#include <SDL.h>

#include <algorithm>
#include <vector>

const std::string TableCut::TABLE_CUT_FRONT_PATH =
    "../Assets/Prototype/TableCut.png";

TableCut::TableCut(Game* game, const std::string& texturePath,
                   std::pair<int, int> gridPos)
    : Table(game, texturePath, gridPos) {}

TableCut* TableCut::NewTableCut(Game* game, LevelTile tile,
                                std::pair<int, int> gridPos) {
    switch (tile) {
        case LevelTile::TileTableCut:
            return new TableCut(game, TABLE_CUT_FRONT_PATH, gridPos);
    }
    return nullptr;
}

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

    std::vector<ItemType> cuttableItems = {ItemType::Tomato};
    // Check item is cuttable
    bool cuttable =
        std::find(std::begin(cuttableItems), std::end(cuttableItems),
                  item->GetItemType()) != std::end(cuttableItems);
    if (!cuttable) return item;

    mItemOnTop = item;
    mItemOnTop->SetPosition(GetPosition() + Vector2(16, 8));
    cutLevel = 0;
    return nullptr;
}

void TableCut::OnItemCut() {
    if (mItemOnTop == nullptr) return;
    if (cutLevel == CUT_LEVEL_MAX) return;

    cutLevel++;  // Add a cut

    // TODO: Add cut animations
    if (cutLevel == CUT_LEVEL_MAX) {
        // Now transforms into finished item
        if (mItemOnTop->GetItemType() == ItemType::Tomato) {
            Item* cutTomato = Item::NewItem(mGame, ItemType::TomatoCut);
            mItemOnTop->SetState(ActorState::Destroy);
            mItemOnTop = cutTomato;
            cutTomato->SetPosition(GetPosition() + Vector2(16, 8));
        }
    }
}