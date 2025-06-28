#include "Deliver.h"

#include <SDL.h>

#include "../Game.h"
#include "Plate.h"

Item* Deliver::SetItemOnTop(Item* item) {
    if (item->GetItemType() != ItemType::Plate) {
        SDL_Log("Needs plate!");
        return item;
    }
    Plate* plate = (Plate*)item;
    std::set<ItemType> recipe = plate->PickItems();

    // Are the items are a wanted dish?
    int points = mGame->GetOrderManager().DeliverOrder(recipe);
    if (points > 0) {
        // TODO
        // YES: Give points

    } else {
        // NO: Beep a sound of mistake
    }

    return plate;
}