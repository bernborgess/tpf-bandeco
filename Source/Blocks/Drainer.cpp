#include "Drainer.h"

#include "../Actors/Plate.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"

const std::string Drainer::DRAINER_PATH = "../Assets/Prototype/DishDrainer.png";
const std::string Drainer::DRAINER_PLATE_PATH =
    "../Assets/Prototype/DishDrainerPlate.png";
const std::string Drainer::DRAINER_PLATES_PATH =
    "../Assets/Prototype/DishDrainerPlates.png";

void Drainer::OnUpdate(float deltaTime) {
    switch (mCleanPlateCount) {
        case 0:
            return mDrawComponent->UpdateTexture(DRAINER_PATH);
        case 1:
            return mDrawComponent->UpdateTexture(DRAINER_PLATE_PATH);
        default:
            return mDrawComponent->UpdateTexture(DRAINER_PLATES_PATH);
    }
}

Item* Drainer::PickItemOnTop() {
    if (mCleanPlateCount < 1) return nullptr;
    mCleanPlateCount--;
    Plate* plate = Plate::NewPlate(mGame);
    return plate;
}