#pragma once

namespace Images {
    
    #include "Card_Large_Back_00.h"
    #include "Card_Large_Back_01.h"
    #include "Card_Large_Back_02.h"
    #include "Card_Large_Back_03.h"
    #include "Card_Large_Back_04.h"
    #include "Card_Large_Back_05.h"
    #include "Card_Large_Back_06.h"
    #include "Card_Large_Back_07.h"
    #include "Card_Large_Back_08.h"
    #include "Card_Large_Back_09.h"
    #include "Card_Large_Back_10.h"
        
    #include "Card_Large_Face_00.h"
    #include "Card_Large_Face_01.h"
    #include "Card_Large_Face_02.h"
    #include "Card_Large_Face_03.h"
    #include "Card_Large_Face_04.h"
    #include "Card_Large_Face_05.h"
    #include "Card_Large_Face_06.h"
    #include "Card_Large_Face_07.h"
    #include "Card_Large_Face_08.h"
    #include "Card_Large_Face_09.h"

    #include "Card_Large_TopCorner.h"
    #include "Card_Large_Highlight.h"
    #include "Card_Large_Bottom_Left.h"
    #include "Card_Large_Bottom_Others.h"
    
    const uint8_t * const Cards_Large_Back[11] = { 
        Card_Large_Back_00, Card_Large_Back_01, Card_Large_Back_02, Card_Large_Back_03, Card_Large_Back_04, 
        Card_Large_Back_05, Card_Large_Back_06, Card_Large_Back_07, Card_Large_Back_08, Card_Large_Back_09, 
        Card_Large_Back_10
    };

    const uint8_t * const Cards_Large_Face[9] = { 
        Card_Large_Face_01, Card_Large_Face_02, Card_Large_Face_03, Card_Large_Face_04, Card_Large_Face_05, 
        Card_Large_Face_06, Card_Large_Face_07, Card_Large_Face_08, Card_Large_Face_09, 
    };

};