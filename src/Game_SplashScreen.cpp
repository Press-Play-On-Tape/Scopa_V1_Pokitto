#include "Game.h"
#include "Pokitto.h"
#include "images/images.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;


void Game::doSplashScreen_Init() { 

    file.openRO("music/SCOP_CDS.img");
    file.seek(SDPpotStart);
    file.read(this->imgBuffer, SDPpotLength);
    
    this->state.setGameState(GameState::SplashScreen);

}

// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void Game::doSplashScreen() { 

    // Decrease button counter that prevents you skipping straight over PPOt splash ..

    if (this->splashScreenVariables.buttonCounter > 0) this->splashScreenVariables.buttonCounter--;

    bool justPressed = (this->splashScreenVariables.buttonCounter > 0 ? false : PC::buttons.pressed(BTN_A) || PC::buttons.pressed(BTN_B) || PC::buttons.pressed(BTN_C));

    if (justPressed > 0) {

        this->state.setGameState(GameState::Title_Init);

    }
    
    if (PC::frameCount % 12 == 0) {

        splashScreenVariables.counter++;
        
        if (splashScreenVariables.counter == 4) {
            
            splashScreenVariables.counter = 0;
            
        }
        
    }
  

    //  Render the state ..

    PD::drawBitmap(44, 54, this->imgBuffer);
    PD::drawBitmap(64, 76, Images::Ppot[splashScreenVariables.counter]);

    if (splashScreenVariables.counter < 2) PD::drawBitmap(163, 72, Images::Ppot_Arrow);

}
