//
// Created by Lucas N. Ferreira on 22/05/25.
//

#include "UIButton.h"

UIButton::UIButton(const std::string &text, class UIFont *font,
                   std::function<void()> onClick, const Vector2 &pos,
                   const Vector2 &size, const Vector3 &color, int pointSize,
                   unsigned wrapLength, const Vector2 &textPos,
                   const Vector2 &textSize, const Vector3 &textColor)
    : UIElement(pos, size, color),
      mOnClick(onClick),
      mHighlighted(false),
      mTextColor(textColor),
      mName(text),
      mText(text, font, pointSize, wrapLength, textPos, textSize, textColor) {}

UIButton::~UIButton() {}

void UIButton::Draw(SDL_Renderer *renderer, const Vector2 &screenPos) {
    Vector2 titlePosition = screenPos + mPosition;

    SDL_Rect titleQuad = {.x = (int)titlePosition.x,
                          .y = (int)titlePosition.y,
                          .w = (int)mSize.x,
                          .h = (int)mSize.y};

    if (mHighlighted) {
        auto &[r, g, b] = mColor;
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderFillRect(renderer, &titleQuad);
        mText.SetColor(mTextColor);
        mText.SetText(mName);
        mText.Draw(renderer, titlePosition + 0.5f * (mSize - mText.GetSize()));
    }

    else {
        auto &[r, g, b] = mColor;
        mText.SetColor(mColor);
        mText.SetText(mName);
        mText.Draw(renderer, titlePosition + 0.5f * (mSize - mText.GetSize()));
    }
}

void UIButton::OnClick() {
    if (mOnClick == nullptr) return;
    mOnClick();
}