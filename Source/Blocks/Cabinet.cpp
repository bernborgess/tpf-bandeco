#include "Cabinet.h"

#include "../Components/DrawComponents/DrawSpriteComponent.h"

const std::string Cabinet::CABINET_EMPTY_PATH =
    "../Assets/Prototype/Cabinet.png";
const std::string Cabinet::CABINET_PLATE_PATH =
    "../Assets/Prototype/CabinetPlate.png";
const std::string Cabinet::CABINET_PLATES_PATH =
    "../Assets/Prototype/CabinetPlates.png";

void Cabinet::OnUpdate(float deltaTime) {
    switch (mDirtyPlateCount) {
        case 0:
            return mDrawComponent->UpdateTexture(CABINET_EMPTY_PATH);
        case 1:
            return mDrawComponent->UpdateTexture(CABINET_PLATE_PATH);
        default:
            return mDrawComponent->UpdateTexture(CABINET_PLATES_PATH);
    }
}

Item* Cabinet::PickItemOnTop() {
    if (mDirtyPlateCount < 1) return nullptr;
    mDirtyPlateCount--;
    return Item::NewItem(mGame, ItemType::PlateDirty);
}