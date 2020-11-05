#include "Game.h"
#include "Pokitto.h"
#include "images/images.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;

void Game::doInstructions_Init() { 

    file.seek(SDInstruction01Start + 2);
    file.read(PD::getBuffer(), SDInstruction01Length - 2);
    this->state.setGameState(GameState::Instructions);
    this->instructionScreenVariables.counter = 0;

}

// ----------------------------------------------------------------------------
//  Handle state updates .. 
//

void Game::doInstructions() { 

    if (PC::buttons.pressed(BTN_LEFT) && this->instructionScreenVariables.counter > 0) {

        this->instructionScreenVariables.counter--;
        file.seek(SDInstruction01Start + (this->instructionScreenVariables.counter * (2 + (220 * 176 / 2))) + 2);
        file.read(PD::getBuffer(), SDInstruction01Length - 2);
        PD::update();
        
    }

    if (PC::buttons.pressed(BTN_RIGHT) && this->instructionScreenVariables.counter < 10) {

        this->instructionScreenVariables.counter++;
        file.seek(SDInstruction01Start + (this->instructionScreenVariables.counter * (2 + (220 * 176 / 2))) + 2);
        file.read(PD::getBuffer(), SDInstruction01Length - 2);
        PD::update();
        
    }

    if (PC::buttons.pressed(BTN_A)) {
        doTitle_Init();
        this->titleSequenceVariables.mode = TitleSequenceMode::Menu;
        this->titleSequenceVariables.menuOption = 2;
        this->state.setGameState(GameState::Title);
    }

}
