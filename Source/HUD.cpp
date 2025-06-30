#include "HUD.h"

#include <iomanip>
#include <sstream>

#include "Game.h"
#include "UIElements/UIText.h"

HUD::HUD(class Game *game, const std::string &fontName)
    : UIScreen(game, fontName) {
    mTimeText = AddText("", Vector2(1200, 760), Vector2(150, 80), Color::Blue);
    mLevelName = AddText("", Vector2(1000, 6), Vector2(360, 60), Color::Blue);
    mPointsCounter =
        AddText("  0", Vector2(100, 769), Vector2(100, 80), Color::Blue);
}

HUD::~HUD() {}

void HUD::SetTime(int time) {
    int minutes = time / 60;
    int second = time % 60;
    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') << minutes << ':';
    ss << std::setw(2) << std::setfill('0') << second;
    std::string time_string = ss.str();
    mTimeText->SetText(time_string);
}

void HUD::SetLevelName(const std::string &levelName) {
    mLevelName->SetText(levelName);
}

void HUD::SetPoints(int points) {
    std::stringstream ss;
    ss << std::setw(3) << std::setfill(' ') << points;
    std::string points_string = ss.str();
    mPointsCounter->SetText(points_string);
}