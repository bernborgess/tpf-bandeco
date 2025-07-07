//
// Created by Lucas N. Ferreira on 08/12/23.
//

#pragma once

#include <string>

#include "UIElements/UIScreen.h"

class HUD : public UIScreen {
   public:
    const int POINT_SIZE = 48;
    const int WORD_HEIGHT = 20.0f;
    const int WORD_OFFSET = 25.0f;
    const int LETTER_WIDTH = 20.0f;
    const int HUD_POS_Y = 10.0f;

    HUD(class Game* game, const std::string& fontName);
    ~HUD();

    // Reinsert the HUD elements
    void SetTime(int time);

    void SetLevelName(const std::string& levelName);

    void SetPoints(int points);

   private:
    // HUD elements
    UIText* mPointsCounter;
    UIText* mPointsCounterAux;
    UIText* mLevelName;
    UIText* mTimeText;
    UIText* mTimeTextAux;
};
