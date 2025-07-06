#pragma once
#include "../Actors/Actor.h"
#include "../Components/DrawComponents/DrawRectangleComponent.h"

class ProgressBar : public Actor {
   public:
    ProgressBar(Game* game, int length);

    void SetShow(bool show) { mDrawComponent->SetShow(show); }
    void SetProgress(double percent) {
        if (percent < 0) percent = 0;
        if (percent > 1) percent = 1;
        mDrawComponent->SetLength(percent *
                                  DrawRectangleComponent::BAR_FULL_LENGTH);
    }

   private:
    class DrawRectangleComponent* mDrawComponent;
    int mLength;
};
