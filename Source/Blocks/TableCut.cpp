#include "TableCut.h"

#include <SDL.h>

#include <algorithm>
#include <vector>

const Vector2 TableCut::TOMATO_OFFSET = Vector2(0, 0);

const std::string TableCut::TABLE_CUT_FRONT_PATH =
    "../Assets/Prototype/TableCut.png";
const std::string TableCut::TABLE_CUT_RIGHT_PATH =
    "../Assets/Prototype/TableCut.png";

TableCut::TableCut(Game* game, const std::string& texturePath,
                   std::pair<int, int> gridPos)
    : Table(game, texturePath, gridPos) {
    mProgressBar = new ProgressBar(game, 32);
    mProgressBar->SetPosition(GetPosition() + Vector2(20, 50));
}

TableCut* TableCut::NewTableCut(Game* game, LevelTile tile,
                                std::pair<int, int> gridPos) {
    switch (tile) {
        case LevelTile::TileTableCut:
        case LevelTile::TileTableCutRight:
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
    mProgressBar->SetShow(false);
    return item;
}

Item* TableCut::SetItemOnTop(Item* item) {
    if (mItemOnTop) {
        // Rejected the item (table full)
        SDL_Log("Rejected the item (table full)");
        return item;
    }

    std::vector<ItemType> cuttableItems = {ItemType::Tomato, ItemType::Lettuce,
                                           ItemType::Meat};
    // Check item is cuttable
    bool cuttable =
        std::find(std::begin(cuttableItems), std::end(cuttableItems),
                  item->GetItemType()) != std::end(cuttableItems);
    if (!cuttable) return item;

    mItemOnTop = item;
    mItemOnTop->SetPosition(GetPosition() + TOMATO_OFFSET);
    cutLevel = 0;
    mProgressBar->SetShow(true);
    SDL_Log("New item on cut. show the progress.");
    mProgressBar->SetProgress(0);
    return nullptr;
}

void TableCut::OnItemCut() {
    if (mItemOnTop == nullptr) return;
    if (cutLevel == CUT_LEVEL_MAX) return;

    cutLevel++;  // Add a cut
    mProgressBar->SetProgress(cutLevel / (double)CUT_LEVEL_MAX);

    // TODO: Add cut animations
    if (cutLevel == CUT_LEVEL_MAX) {
        // Now transforms into finished item
        if (mItemOnTop->GetItemType() == ItemType::Tomato) {
            // TODO: let the cut tomato be bigger than 32x32
            Item* cutTomato = Item::NewItem(mGame, ItemType::TomatoCut);
            mItemOnTop->SetState(ActorState::Destroy);
            mItemOnTop = cutTomato;
            cutTomato->SetPosition(GetPosition() + TOMATO_OFFSET);
            mProgressBar->SetShow(false);
        }
        if (mItemOnTop->GetItemType() == ItemType::Lettuce) {
            Item* cutLettuce = Item::NewItem(mGame, ItemType::LettuceCut);
            mItemOnTop->SetState(ActorState::Destroy);
            mItemOnTop = cutLettuce;
            cutLettuce->SetPosition(GetPosition() + TOMATO_OFFSET);
            mProgressBar->SetShow(false);
        }
        if (mItemOnTop->GetItemType() == ItemType::Meat) {
            Item* cutMeat = Item::NewItem(mGame, ItemType::MeatCut);
            mItemOnTop->SetState(ActorState::Destroy);
            mItemOnTop = cutMeat;
            cutMeat->SetPosition(GetPosition() + TOMATO_OFFSET);
            mProgressBar->SetShow(false);
        }
    }
}