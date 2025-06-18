//
// Created by Lucas N. Ferreira on 08/12/23.
//

#include "HUD.h"

#include <iomanip>
#include <sstream>

#include "Game.h"
#include "UIElements/UIText.h"

HUD::HUD(class Game *game, const std::string &fontName)
    : UIScreen(game, fontName) {
    Vector2 cw = Vector2(LETTER_WIDTH, 0), wh = Vector2(0, WORD_HEIGHT),
            right = Vector2(game->GetWindowWidth(), HUD_POS_Y),
            left = Vector2(0, HUD_POS_Y);
    AddText("Time", right - 8 * cw, 4 * cw + wh);
    mTimeText = AddText("400", right - 7 * cw + wh, 3 * cw + wh);
    AddText("World", right - 16 * cw, 5 * cw + wh);
    mLevelName = AddText("1-1", right - 15 * cw + wh, 3 * cw + wh);
    AddText("Player", left + 2 * cw, 5 * cw + wh);
    AddText("000000", left + 2 * cw + wh, 6 * cw + wh);
}

HUD::~HUD() {}

void HUD::SetTime(int time) {
    std::stringstream ss;
    ss << std::setw(3) << std::setfill('0') << time;
    std::string time_string = ss.str();
    mTimeText->SetText(time_string);
}

void HUD::SetLevelName(const std::string &levelName) {
    mLevelName->SetText(levelName);
}