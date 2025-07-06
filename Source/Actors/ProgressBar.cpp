#include "ProgressBar.h"

#include "../Math.h"

ProgressBar::ProgressBar(Game* game, int length)
    : Actor(game), mLength(length) {
    mDrawComponent = new DrawRectangleComponent(this);
    mDrawComponent->SetLength(0);
}