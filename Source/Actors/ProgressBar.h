#pragma once
#include "../Actors/Actor.h"
#include "../Components/DrawComponents/DrawRectangleComponent.h"

class ProgressBar : public Actor {
   public:
    ProgressBar(Game* game, int length);

   private:
    class DrawRectangleComponent* mDrawComponent;
    class DrawRectangleComponent* mForeground;
    int mLength;
};
