#include "Table.h"

Table::Table(Game* game, const std::string& texturePath,
             std::pair<int, int> gridPos)
    : Block(game, texturePath, gridPos), mItemOnTop(nullptr) {}

Item* Table::PickItemOnTop() {
    if (!mItemOnTop) return nullptr;
    Item* item = mItemOnTop;
    mItemOnTop = nullptr;
    return item;
}

bool Table::HasItemOnTop() { return mItemOnTop != nullptr; }

Item* Table::SetItemOnTop(Item* item) {
    if (mItemOnTop) {
        // Rejected the item (table full)
        return item;
    }
    // Accepted the item, returns nothing
    mItemOnTop = item;
    mItemOnTop->SetPosition(GetPosition() + Vector2(16, 8));
    return nullptr;
}