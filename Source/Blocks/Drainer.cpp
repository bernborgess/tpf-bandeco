#include "Drainer.h"

#include "../Actors/Plate.h"

Item* Drainer::PickItemOnTop() {
    if (mCleanPlateCount < 1) return nullptr;
    mCleanPlateCount--;
    Plate* plate = Plate::NewPlate(mGame);
    return plate;
}