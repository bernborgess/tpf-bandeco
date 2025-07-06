#include "ProgressBar.h"

#include "../Math.h"

ProgressBar::ProgressBar(Game* game) : Actor(game) {
    mDrawComponent = new DrawRectangleComponent(this);
    mDrawComponent->SetLength(0);
}