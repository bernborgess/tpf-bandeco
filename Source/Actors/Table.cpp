#include "Table.h"

#include <SDL.h>

#include "Plate.h"
#include "Pot.h"

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
    if (!item) return item;

    if (mItemOnTop) {
        // Check if item on top is a Pot or Plate
        if (mItemOnTop->GetItemType() == ItemType::Pot) {
            Pot* pot = (Pot*)mItemOnTop;
            // Check if player has a plate on his hand
            if (item->GetItemType() == ItemType::Plate) {
                Plate* plate = (Plate*)item;
                // Will move food from pot to plate, if plate accepts
                Item* itemInside = pot->PickItem();
                pot->PutItem(plate->PutItem(itemInside));
                return plate;
            }
            return pot->PutItem(item);
        }
        if (mItemOnTop->GetItemType() == ItemType::Plate) {
            Plate* plate = (Plate*)mItemOnTop;
            // Check if player has a Pot on his hand
            if (item->GetItemType() == ItemType::Pot) {
                Pot* pot = (Pot*)item;
                Item* itemInside = pot->PickItem();
                pot->PutItem(plate->PutItem(itemInside));
                return pot;
            }
            return plate->PutItem(item);
        }

        // Rejected the item (table full)
        return item;
    }
    // Accepted the item, returns nothing
    mItemOnTop = item;
    mItemOnTop->SetPosition(GetPosition() + Vector2(16, 8));
    return nullptr;
}