#pragma once

#include <string>

#include "Actor.h"
#include "Item.h"

class Plate : public Item {
   public:
    static Plate* NewPlate(Game* game);
    Item* PutItem(Item* item);
    // TODO: Think about this action
    void Deliver();
    void OnUpdate(float deltaTime) override;

   private:
    Plate(Game* game, const std::string& texturePath);
    std::vector<Item*> mItems;
};