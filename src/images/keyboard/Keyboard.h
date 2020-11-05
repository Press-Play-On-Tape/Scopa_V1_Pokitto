#pragma once

namespace Images {
    
    #include "FaceSmiley.h"
    #include "FaceFrown.h"

    #include "Keyboard_Full.h"
    #include "Keyboard_Space.h"
    #include "Keyboard_Highlight.h"

    #include "Keyboard_Outline_Normal.h"
    #include "Keyboard_Outline_Enter.h"
    #include "Keyboard_Outline_Space.h"
    #include "Keyboard_Outline_Del.h"

    const uint8_t * const Keyboard_Outlines[4] = { Keyboard_Outline_Normal, Keyboard_Outline_Enter, Keyboard_Outline_Space, Keyboard_Outline_Del };

};