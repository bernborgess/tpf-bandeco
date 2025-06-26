#include "Trash.h"

#include "Pot.h"

Item* Trash::DiscardItem(Item* item) {
    if (!item) return nullptr;
    if (item->GetItemType() == ItemType::Pot) {
        Pot* pot = (Pot*)item;
        Item* itemInside = pot->PickItem();
        if (itemInside) {
            delete itemInside;
        }
        return item;
    } else {
        delete item;
        return nullptr;
    }
}