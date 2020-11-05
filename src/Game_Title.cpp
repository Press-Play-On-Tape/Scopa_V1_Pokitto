#include "Game.h"
#include "Pokitto.h"
#include "images/images.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;

void Game::doTitle_Init() { 

    file.seek(SDTitleStart + 2);
    file.read(PD::getBuffer(), SDTitleLength - 2);

    file.seek(SDTitleReplaceStart);
    file.read(this->imgBuffer, SDTitleReplaceLength);

    this->state.reset();
    this->state.setGameState(GameState::Title);
    this->titleSequenceVariables.reset();

    this->playTheme(0);

}

// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
#define TITLE_ROTATE_SPEED 2

void Game::doTitle() { 

    const uint8_t spinOffset[21] = { 0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 79, 72, 64, 56, 48, 40, 32, 24, 16, 8, 0 };
    const uint8_t textOffset[21] = { 36, 32, 29, 25, 22, 16, 14, 11, 7, 4, 0, 5, 7, 11, 14, 18, 22, 25, 29, 32, 36 };

    if (this->titleSequenceVariables.delay > 0) this->titleSequenceVariables.delay--;

    if (PC::frameCount % 2 == 0) this->titleSequenceVariables.star++;
    if (this->titleSequenceVariables.star == 170) this->titleSequenceVariables.star = 0;

    switch (this->titleSequenceVariables.star) {

        case 9:
        case 49:
        case 79:
        case 124:
        case 144:
        case 174:
        case 209:
        case 249:
            this->titleSequenceVariables.colour = random(0, 4);
            break;

        case 10 ... 10 + 6:
            PD::drawBitmap(20, 10, Images::Stars[this->titleSequenceVariables.colour][this->titleSequenceVariables.star - 10]);
            break;

        case 30 ... 30 + 6:
            PD::drawBitmap(170, 30, Images::Stars[this->titleSequenceVariables.colour][this->titleSequenceVariables.star - 30]);
            break;

        case 50 ... 50 + 6:
            PD::drawBitmap(50, 40, Images::Stars[this->titleSequenceVariables.colour][this->titleSequenceVariables.star - 50]);
            break;

        case 70 ... 70 + 6:
            PD::drawBitmap(208, 50, Images::Stars[this->titleSequenceVariables.colour][this->titleSequenceVariables.star - 70]);
            break;
            
        case 90 ... 90 + 6:
            PD::drawBitmap(30, 20, Images::Stars[this->titleSequenceVariables.colour][this->titleSequenceVariables.star - 90]);
            break;

        case 110 ... 110 + 6:
            PD::drawBitmap(140, 0, Images::Stars[this->titleSequenceVariables.colour][this->titleSequenceVariables.star - 110]);
            break;
            
        case 130 ... 130 + 6:
            PD::drawBitmap(0, 50, Images::Stars[this->titleSequenceVariables.colour][this->titleSequenceVariables.star - 130]);
            break;

        case 150 ... 150 + 6:
            PD::drawBitmap(208, 5, Images::Stars[this->titleSequenceVariables.colour][this->titleSequenceVariables.star - 150]);
            break;

    }


    switch (this->titleSequenceVariables.mode) {

        case TitleSequenceMode::Normal:

            if (this->titleSequenceVariables.delay == 0 && (PC::buttons.pressed(BTN_A)|| PC::buttons.pressed(BTN_B))) {
                
                this->titleSequenceVariables.spinCounter = 11;
                this->titleSequenceVariables.mode = TitleSequenceMode::Menu;
                
            }

            break;

        case TitleSequenceMode::Menu:

            switch (this->titleSequenceVariables.spinCounter) {

                case 11:
                    PD::drawBitmap(30 + spinOffset[this->titleSequenceVariables.spinCounter - 1], 45, Images::TitlePanels[this->titleSequenceVariables.spinCounter - 1]);
                    if (PC::frameCount % TITLE_ROTATE_SPEED == 0) {
                        this->titleSequenceVariables.spinCounter--;
                    }
                    break;

                case 1 ... 10:
                    PD::drawBitmap(30 + spinOffset[this->titleSequenceVariables.spinCounter - 1], 45, Images::TitlePanels[this->titleSequenceVariables.spinCounter - 1]);
                    PD::drawBitmap(30 + spinOffset[this->titleSequenceVariables.spinCounter - 1] + textOffset[this->titleSequenceVariables.spinCounter - 1], 57, Images::TitleTexts[this->titleSequenceVariables.spinCounter - 1]);
                    if (PC::frameCount % TITLE_ROTATE_SPEED == 0) {
                        this->titleSequenceVariables.spinCounter--;
                    }
                    break;

                case 0:

                    PD::drawBitmap(30, 45, Images::TitlePanels[0]);
                    PD::drawBitmap(30 + textOffset[0], 57, Images::TitleTexts[0]);
                    PD::drawBitmap(50, 56 + (this->titleSequenceVariables.menuOption * 13), Images::TitleCoins[this->titleSequenceVariables.coinFrame]);

                    if (PC::frameCount % 8 == 0) {
                        this->titleSequenceVariables.coinFrame++;
                        if (this->titleSequenceVariables.coinFrame == 5) this->titleSequenceVariables.coinFrame = 0;
                    }

                    switch (this->titleSequenceVariables.menuOption) {

                        case 0:
                            PD::setColor(7, 14);
                            PD::setCursor(65, 57);
                            PD::print("Solo Mode");
                            break;

                        case 1:
                            PD::setColor(7, 14);
                            PD::setCursor(65, 70);
                            PD::print("Pass & Play");
                            break;

                        case 2:
                            PD::setColor(7, 14);
                            PD::setCursor(65, 83);
                            PD::print("Instructions");
                            break;
                            
                    }

                    if (PC::buttons.pressed(BTN_UP) && this->titleSequenceVariables.menuOption > 0) {
                        
                        this->titleSequenceVariables.menuOption--;
                        
                    }

                    if (PC::buttons.pressed(BTN_DOWN) && this->titleSequenceVariables.menuOption < 2) {
                        
                        this->titleSequenceVariables.menuOption++;
                        
                    }

                    if (PC::buttons.pressed(BTN_A)) {

                        switch (this->titleSequenceVariables.menuOption) {

                            case 0:
                                this->state.setGameMode(GameMode::SinglePlayer);
                                this->state.setGameState(GameState::Play_Init);
                                break;

                            case 1:
                                this->titleSequenceVariables.menuOption = 0;
                                this->titleSequenceVariables.spinCounter = 0;
                                this->titleSequenceVariables.mode = TitleSequenceMode::PlayAndPass;
                                break;

                            case 2:
                                this->state.setGameState(GameState::Instructions_Init);
                                break;

                        }
                        
                    }

                    if (PC::buttons.pressed(BTN_B)) {
        
                        doTitle_Init();
        
                    }
                    
                    break;

            }

            break;


        case TitleSequenceMode::PlayAndPass:

            switch (this->titleSequenceVariables.spinCounter) {

                case 0 ... 9:
                case 11 ... 20:
                    PD::drawBitmap(30, 43, this->imgBuffer);
                    PD::drawBitmap(30 + spinOffset[this->titleSequenceVariables.spinCounter], 45, Images::TitlePanels[this->titleSequenceVariables.spinCounter]);
                    PD::drawBitmap(30 + spinOffset[this->titleSequenceVariables.spinCounter] + textOffset[this->titleSequenceVariables.spinCounter], 57, Images::TitleTexts[this->titleSequenceVariables.spinCounter]);
                    if (PC::frameCount % TITLE_ROTATE_SPEED == 0) {
                        this->titleSequenceVariables.spinCounter++;
                    }
                    break;

                case 10:
                    PD::drawBitmap(30, 43, this->imgBuffer);
                    PD::drawBitmap(30 + spinOffset[this->titleSequenceVariables.spinCounter], 45, Images::TitlePanels[this->titleSequenceVariables.spinCounter]);
                    if (PC::frameCount % TITLE_ROTATE_SPEED == 0) {
                        this->titleSequenceVariables.spinCounter++;
                    }
                    break;

                case 21:

                    PD::drawBitmap(30, 45, Images::TitlePanels[20]);
                    PD::drawBitmap(30 + textOffset[20], 57, Images::TitleTexts[20]);
                    PD::drawBitmap(50, 56 + (this->titleSequenceVariables.menuOption * 13), Images::TitleCoins[this->titleSequenceVariables.coinFrame]);

                    if (PC::frameCount % 8 == 0) {
                        this->titleSequenceVariables.coinFrame++;
                        if (this->titleSequenceVariables.coinFrame == 5) this->titleSequenceVariables.coinFrame = 0;
                    }



                    switch (this->titleSequenceVariables.menuOption) {

                        case 0:
                            PD::setColor(7, 14);
                            PD::setCursor(65, 57);
                            PD::print("Play Now");
                            break;

                        case 1:
                            PD::setColor(7, 14);
                            PD::setCursor(65, 70);
                            PD::print("Change Names");
                            break;

                    }

                    if (PC::buttons.pressed(BTN_UP) && this->titleSequenceVariables.menuOption > 0) {
                        
                        this->titleSequenceVariables.menuOption--;
                        
                    }

                    if (PC::buttons.pressed(BTN_DOWN) && this->titleSequenceVariables.menuOption < 1) {
                        
                        this->titleSequenceVariables.menuOption++;
                        
                    }

                    if (PC::buttons.pressed(BTN_A)) {

                        switch (this->titleSequenceVariables.menuOption) {

                            case 0:
                                this->state.setGameMode(GameMode::PlayAndPass);
                                this->state.setGameState(GameState::Play_Init);
                                break;

                            case 1:
                                this->state.setGameMode(GameMode::PlayAndPass);
                                this->state.setGameState(GameState::Play_PlayerOneName_Init);
                                break;

                        }
                        
                    }

                    if (PC::buttons.pressed(BTN_B)) {
        
                        doTitle_Init();
        
                    }
                    
                    break;

            }

            break;


    }


}
