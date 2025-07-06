#include "DrawRectangleComponent.h"

#include "../../Actors/Actor.h"

DrawRectangleComponent::DrawRectangleComponent(class Actor* owner)
    : DrawComponent(owner, 200), mShow(false) {
    mSize = Vector2(BAR_FULL_LENGTH, 8);
    mLength = 0;
}

void DrawRectangleComponent::Draw(SDL_Renderer* renderer,
                                  const Vector3& modColor) {
    if (!mShow) return;
    // Background rectangle
    {
        auto [r, g, b] = Color::Black;
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        const Vector2 pos = mOwner->GetPosition();
        const SDL_Rect rect = {.x = (int)pos.x,
                               .y = (int)pos.y,
                               .w = (int)mSize.x,
                               .h = (int)mSize.y};
        SDL_RenderFillRect(renderer, &rect);
    }

    // Foreground rectangle
    {
        auto [r, g, b] = Color::Green;
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        const Vector2 pos = mOwner->GetPosition();
        const SDL_Rect rect = {
            .x = (int)pos.x, .y = (int)pos.y, .w = mLength, .h = (int)mSize.y};
        SDL_RenderFillRect(renderer, &rect);
    }
}
