#pragma once
#include "../Actors/Actor.h"
#include "../Components/DrawComponents/DrawRectangleComponent.h"

class ProgressBar : public Actor {
   public:
    ProgressBar(Game* game);

    void SetShow(bool show) { mDrawComponent->SetShow(show); }
    void SetProgress(double percent, bool altColor = false) {
        if (percent < 0) percent = 0;
        if (percent > 1) percent = 1;
        mDrawComponent->SetLength(percent *
                                  DrawRectangleComponent::BAR_FULL_LENGTH);
        if (altColor)
            SetColor(Color::Yellow);
        else
            SetColor(Color::Green);
    }
    void SetColor(Vector3 color) { mDrawComponent->SetColor(color); }

   private:
    class DrawRectangleComponent* mDrawComponent;
};
