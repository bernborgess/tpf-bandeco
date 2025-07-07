#include "Cabinet.h"

Item* Cabinet::PickItemOnTop() {
    if (mDirtyPlateCount < 1) return nullptr;
    mDirtyPlateCount--;
    return Item::NewItem(mGame, ItemType::PlateDirty);
}