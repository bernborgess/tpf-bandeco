//
// Created by Lucas N. Ferreira on 22/05/25.
//

#include "UIText.h"

#include "UIFont.h"

UIText::UIText(const std::string &text, class UIFont *font, int pointSize,
               const unsigned wrapLength, const Vector2 &pos,
               const Vector2 &size, const Vector3 &color)
    : UIElement(pos, size, color),
      mFont(font),
      mPointSize(pointSize),
      mWrapLength(wrapLength),
      mTextTexture(nullptr) {
    SetText(text);
}

UIText::~UIText() {}

void UIText::SetText(const std::string &text) {
    if (mTextTexture) {
        SDL_DestroyTexture(mTextTexture);
        mTextTexture = nullptr;
    }

    mTextTexture = mFont->RenderText(text, mColor, mPointSize, mWrapLength);
    mText = text;
}

void UIText::Draw(SDL_Renderer *renderer, const Vector2 &screenPos) {
    Vector2 finalPosition = mPosition + screenPos;
    SDL_Rect titleQuad = {.x = (int)finalPosition.x,
                          .y = (int)finalPosition.y,
                          .w = (int)mSize.x,
                          .h = (int)mSize.y};

    SDL_RenderCopyEx(renderer, mTextTexture, nullptr, &titleQuad, 0, nullptr,
                     SDL_FLIP_NONE);
}