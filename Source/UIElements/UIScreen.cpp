
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
    for (auto &text : mTexts) {
        text->Draw(renderer, mPos);
    }

    for (auto &button : mButtons) {
        button->Draw(renderer, mPos);
    }

    for (auto &image : mImages) {
        image->Draw(renderer, mPos);
    }
}

void UIScreen::ProcessInput(const uint8_t *keys) {}

void UIScreen::HandleKeyPress(int key) {
    size_t n = mButtons.size();
    if (n < 1) return;
    switch (key) {
        case SDLK_w: {
            mButtons[mSelectedButtonIndex]->SetHighlighted(false);
            mSelectedButtonIndex += n - 1;
            mSelectedButtonIndex %= n;
            mButtons[mSelectedButtonIndex]->SetHighlighted(true);
            break;
        }
        case SDLK_s: {
            mButtons[mSelectedButtonIndex]->SetHighlighted(false);
            mSelectedButtonIndex += 1;
            mSelectedButtonIndex %= n;
            mButtons[mSelectedButtonIndex]->SetHighlighted(true);
            break;
        }
        case SDLK_RETURN: {
            if (mSelectedButtonIndex < 0 || mSelectedButtonIndex >= n) return;
            mButtons[mSelectedButtonIndex]->OnClick();
            break;
        }
    }
}

void UIScreen::Close() { mState = UIState::Closing; }

UIText *UIScreen::AddText(const std::string &name, const Vector2 &pos,
                          const Vector2 &dims, const int pointSize,
                          const int unsigned wrapLength) {
    UIText *t = new UIText(name, mFont, pointSize, wrapLength, pos, dims);
    mTexts.push_back(t);
    return t;
}

UIButton *UIScreen::AddButton(const std::string &name, const Vector2 &pos,
                              const Vector2 &dims,
                              std::function<void()> onClick) {
    Vector3 orange = Vector3(200, 100, 0);
    UIButton *b = new UIButton(name, mFont, onClick, pos, dims, orange);
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