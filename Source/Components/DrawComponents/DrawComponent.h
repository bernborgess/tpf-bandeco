//
// Created by Lucas N. Ferreira on 03/08/23.
//

#pragma once
#include <SDL.h>

#include <vector>

#include "../../Math.h"
#include "../Component.h"

class DrawComponent : public Component {
   public:
    // (Lower draw order corresponds with further back)
    explicit DrawComponent(class Actor* owner, int drawOrder = 100);
    ~DrawComponent() override;

    virtual void Draw(SDL_Renderer* renderer,
                      const Vector3& modColor = Color::White);

    bool IsVisible() const { return mIsVisible; }
    void SetIsVisible(const bool isVisible) { mIsVisible = isVisible; }

    int GetDrawOrder() const { return mDrawOrder; }

    void ResetAbove();
    void SetAbove();

   protected:
    bool mIsVisible;
    int mDrawOrder;
    bool mIsAbove;
};
