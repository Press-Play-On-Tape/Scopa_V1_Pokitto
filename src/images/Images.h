#pragma once

#include "ppot/PPOT.h"
#include "cards/large/LargeCards.h"
#include "cards/small/SmallCards.h"
#include "keyboard/Keyboard.h"
#include "stars/Stars.h"
#include "title/Titles.h"

namespace Images {

    #include "Hand.h"
    #include "Hand_Large.h"
    #include "Scopa.h"
    #include "cards/deck/Card_Deck.h"
    #include "Score.h"
    #include "Panel.h"

    #include "TalkBubble.h"
    #include "TalkingBubble_00.h"
    #include "TalkingBubble_01.h"
    #include "TalkingBubble_02.h"

    #include "PressAButton.h"

    const uint8_t * const TalkingBubbles[3] = { TalkingBubble_00, TalkingBubble_01, TalkingBubble_02 };
};