#pragma once

namespace Images {
    
    #include "Card_Small_Front.h"
    #include "Card_Small_Front_Selected.h"
    #include "Card_Small_Suit_Club.h"
    #include "Card_Small_Suit_Coin.h"
    #include "Card_Small_Suit_Sword.h"
    #include "Card_Small_Suit_Vase.h"
    #include "Card_Small_Back.h"

    #include "Card_Small_Deck_00.h"
    #include "Card_Small_Deck_01.h"
    #include "Card_Small_Deck_02.h"
    #include "Card_Small_Deck_03.h"
    #include "Card_Small_Deck_04.h"

    #include "Card_Small_Number_01.h"
    #include "Card_Small_Number_02.h"
    #include "Card_Small_Number_03.h"
    #include "Card_Small_Number_04.h"
    #include "Card_Small_Number_05.h"
    #include "Card_Small_Number_06.h"
    #include "Card_Small_Number_07.h"
    #include "Card_Small_Number_08.h"
    #include "Card_Small_Number_09.h"
    #include "Card_Small_Number_10.h"
    #include "Card_Small_Underline.h"

    const uint8_t * const Cards_Small_Suits[4] = { 
        Card_Small_Suit_Coin, Card_Small_Suit_Vase, Card_Small_Suit_Club, Card_Small_Suit_Sword, 
    };

    const uint8_t * const Cards_Small_Numbers[10] = { 
        
        Card_Small_Number_01, Card_Small_Number_02, Card_Small_Number_03, Card_Small_Number_04, Card_Small_Number_05, 
        Card_Small_Number_06, Card_Small_Number_07, Card_Small_Number_08, Card_Small_Number_09, Card_Small_Number_10, 
        
    };

    const uint8_t * const Card_Small_Deck[6] = { 
        Card_Small_Deck_00, Card_Small_Deck_01, Card_Small_Deck_02, Card_Small_Deck_03, Card_Small_Deck_04, Card_Small_Deck_04, // 5 Deals
    };

};