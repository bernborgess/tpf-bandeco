#pragma once
#include "DrawComponent.h"

class DrawRectangleComponent : public DrawComponent {
   public:
    DrawRectangleComponent(class Actor* owner);

    void Draw(SDL_Renderer* renderer,
              const Vector3& modColor = Color::White) override;

    void SetLength(int length) { mLength = length; }
    void SetShow(bool show) { mShow = show; }

    static const int BAR_FULL_LENGTH = 32;

   protected:
    Vector2 mSize;
    int mLength;
    bool mShow;
};