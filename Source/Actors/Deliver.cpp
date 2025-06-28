#include "Deliver.h"

#include <SDL.h>

#include "Plate.h"

Item* Deliver::SetItemOnTop(Item* item) {
    if (item->GetItemType() != ItemType::Plate) {
        SDL_Log("Needs plate!");
        return item;
    }
    Plate* plate = (Plate*)item;
    std::vector<Item*> items = plate->PickItems();
    // TODO: The items are a wanted dish?
    // YES: Give points
    // NO: Beep a sound of mistake

    // Delete the items
    for (auto itemInside : items) {
        itemInside->SetState(ActorState::Destroy);
    }

    return plate;
}