
#include "UIScreen.h"

#include "../Game.h"
#include "UIFont.h"

UIScreen::UIScreen(Game *game, const std::string &fontName)
    : mGame(game),
      mPos(0.f, 0.f),
      mSize(0.f, 0.f),
      mState(UIState::Active),
      mSelectedButtonIndex(-1) {
    mGame->PushUI(this);
    mFont = mGame->LoadFont(fontName);
}

UIScreen::~UIScreen() {
    for (auto &text : mTexts) {
        delete text;
    }
    mTexts.clear();

    for (auto &button : mButtons) {
        delete button;
    }
    mButtons.clear();

    for (auto &image : mImages) {
        delete image;
    }
    mImages.clear();
}

void UIScreen::Update(float deltaTime) {}

void UIScreen::Draw(SDL_Renderer *renderer) {
    for (auto &image : mImages) {
        image->Draw(renderer, mPos);
    }

    for (auto &text : mTexts) {
        text->Draw(renderer, mPos);
    }

    for (auto &button : mButtons) {
        button->Draw(renderer, mPos);
    }
}

void UIScreen::ProcessInput(const uint8_t *keys) {}

void UIScreen::HandleKeyPress(int scanCode) {
    size_t n = mButtons.size();
    if (n < 1) return;
    switch (scanCode) {
        case SDL_SCANCODE_UP:
        case SDL_SCANCODE_W: {
            mButtons[mSelectedButtonIndex]->SetHighlighted(false);
            mSelectedButtonIndex += n - 1;
            mSelectedButtonIndex %= n;
            mButtons[mSelectedButtonIndex]->SetHighlighted(true);
            break;
        }
        case SDL_SCANCODE_DOWN:
        case SDL_SCANCODE_S: {
            mButtons[mSelectedButtonIndex]->SetHighlighted(false);
            mSelectedButtonIndex += 1;
            mSelectedButtonIndex %= n;
            mButtons[mSelectedButtonIndex]->SetHighlighted(true);
            break;
        }
        case SDL_SCANCODE_SPACE:
        case SDL_SCANCODE_RETURN: {
            if (mSelectedButtonIndex < 0 || mSelectedButtonIndex >= n) return;
            mButtons[mSelectedButtonIndex]->OnClick();
            break;
        }
    }
}

void UIScreen::Close() { mState = UIState::Closing; }

UIText *UIScreen::AddText(const std::string &name, const Vector2 &pos,
                          const Vector2 &dims, const Vector3 &color,
                          const int pointSize, const int unsigned wrapLength) {
    UIText *t =
        new UIText(name, mFont, pointSize, wrapLength, pos, dims, color);
    mTexts.push_back(t);
    return t;
}

UIButton *UIScreen::AddButton(const std::string &name, const Vector2 &pos,
                              const Vector2 &dims,
                              std::function<void()> onClick,
                              const Vector3 &backgroundColor, int pointSize,
                              int wrapLength, const Vector2 &textPos,
                              const Vector2 &textSize,
                              const Vector3 &textColor) {
    UIButton *b =
        new UIButton(name, mFont, onClick, pos, dims, backgroundColor,
                     pointSize, wrapLength, textPos, textSize, textColor);
    mButtons.push_back(b);

    if (mButtons.size() == 1) {
        mSelectedButtonIndex = 0;
        mButtons[0]->SetHighlighted(true);
    }

    return b;
}

UIImage *UIScreen::AddImage(const std::string &imagePath, const Vector2 &pos,
                            const Vector2 &dims, const Vector3 &color) {
    UIImage *image = new UIImage(mGame, imagePath, pos, dims, color);
    mImages.push_back(image);
    return image;
}