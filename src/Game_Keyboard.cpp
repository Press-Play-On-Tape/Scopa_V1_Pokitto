#include "Game.h"
#include "Pokitto.h"
#include "images/images.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;


const uint8_t keyboardPosX[40] = {   5,  25,  45,  65,  85, 105, 125, 145, 165, 185,  
                                    14,  34,  54,  74,  94, 114, 134, 154, 174, 183,
                                    23,  43,  63,  83, 103, 123, 143, 163, 183, 183,
                                     9,  29,  49,  49,  49,  49,  49,  49, 169, 169 };
                                     
const uint8_t keyboardPosY[40] = { 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 
                                   120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 
                                   138, 138, 138, 138, 138, 138, 138, 138, 120, 120, 
                                   156, 156, 156, 156, 156, 156, 156, 156, 156, 156 };
                                     
const uint8_t keyboardHighlight[40] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                        0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                                        0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
                                        0, 0, 2, 2, 2, 2, 2, 2, 3, 3, };
                                     
const uint8_t keyboardLetterIdx[40] = { 16, 22,  4, 17, 19, 24, 20,  8, 14, 15,
                                         0, 18,  3,  5,  6,  7,  9, 10, 11, 28,
                                        25, 23,  2, 21,  1, 13, 12, 31, 28, 28,
                                        26, 27, 30, 30, 30, 30, 30, 30, 29, 29, };


void Game::doPlay_Keyboard_Init(KeyboardMode mode) { 

    switch (mode) {

        case KeyboardMode::Message:
            {
                this->renderBackground(this->getBackgroundColourByTurn(), 0, 0, 220, 178);
                this->renderTopHand(this->state.getCurrentTurn() == PLAYER_ONE ? PLAYER_TWO : PLAYER_ONE);
                this->renderTable(false);
                this->renderDeck();
                this->renderPlayerName(this->state.getCurrentTurn());
                PD::setColor(6, 0);
                PD::drawFastHLine(0, 77, 220);
                this->renderScore(this->state.getCurrentTurn());
                PD::update();

                KeyboardSequenceVariables &keyboardSequenceVariables = this->state.getKeyboardSequenceVariables(this->state.getCurrentTurn() == PLAYER_ONE ? PLAYER_TWO : PLAYER_ONE);
                keyboardSequenceVariables.reset();

                for (uint8_t i = 0; i < 20; i++) {

                    keyboardSequenceVariables.message[i] = ASCII_SPACE;

                }

                this->state.setGameState(GameState::Play_Keyboard);
            }
            break;

        case KeyboardMode::PlayerOneName:
        case KeyboardMode::PlayerTwoName:
            {
                PD::drawBitmap(50, 69, Images::TitleCoins[0]);
                KeyboardSequenceVariables &keyboardSequenceVariables = this->state.getKeyboardSequenceVariables(2);
                keyboardSequenceVariables.reset();

                for (uint8_t i = 0; i < 20; i++) {

                    keyboardSequenceVariables.message[i] = ASCII_SPACE;

                }

                uint8_t j = 0;

                for (uint8_t i = 0; i < 8; i++) {

                    keyboardSequenceVariables.message[i] = this->cookie->getPlayerChar(static_cast<uint8_t>(mode) - 1, i);

                    if (this->cookie->getPlayerChar(static_cast<uint8_t>(mode) - 1, i) != 32) {
                        j = i;
                    }

                }

                keyboardSequenceVariables.messageIdx = j + 1;

                switch (mode) {

                    case KeyboardMode::PlayerOneName:   
                        this->state.setGameState(GameState::Play_PlayerOneName);
                        break;

                    case KeyboardMode::PlayerTwoName:   
                        this->state.setGameState(GameState::Play_PlayerTwoName);
                        break;

                }
                        

            }
            break;

    }

}

void Game::doPlay_Keyboard_RenderKey(char charToRender, uint8_t x) { 

    switch (charToRender) {

        case ASCII_START ... ASCII_END:
        case ASCII_PERIOD:
            PD::setCursor(x, 87);
            PD::print(charToRender);
            break;

        case KEY_SMILEY_FACE:
            PD::drawBitmap(x, 87, Images::FaceSmiley);
            break;

        case KEY_FROWN_FACE:
            PD::drawBitmap(x, 87, Images::FaceFrown);
            break;

    }

}


void Game::doPlay_Keyboard(KeyboardMode mode) { 

    uint8_t keyboardIndex = 0;
    uint8_t messageLength = 0;

    switch (mode) {
    
        case KeyboardMode::Message:
            keyboardIndex = this->state.getCurrentTurn() == PLAYER_ONE ? PLAYER_TWO : PLAYER_ONE;
            PD::setColor(this->getBackgroundColourByTurn());
            PD::fillRect(0, 78, 220, 100);
            PD::setColor(6, 0);
            PD::drawRect(17, 82, 186, 16);
            PD::setColor(0, 0);
            PD::fillRect(20, 85, 180, 10);
            PD::setColor(8, 0);
            break;

        case KeyboardMode::PlayerOneName:
        case KeyboardMode::PlayerTwoName:

            PD::setColor(0, 0);
            PD::fillRect(0, 78, 220, 100);
            PD::setColor(6, 0);
            PD::drawRect(85, 82, 100, 16);
            PD::setColor(0, 0);
            PD::fillRect(88, 85, 94, 10);
            PD::setColor(10, 14);
            PD::drawFastVLine(37, 78, 2);
            PD::drawFastHLine(0, 80, 37);
            PD::drawFastVLine(182, 78, 2);
            PD::drawFastHLine(182, 80, 40);
            PD::setColor(9, 14);
            PD::drawFastVLine(36, 78, 1);
            PD::drawFastHLine(0, 79, 36);
            PD::drawFastVLine(183, 78, 1);
            PD::drawFastHLine(183, 79, 40);
            PD::setColor(7, 14);

            switch (mode) {

                case KeyboardMode::PlayerOneName:
                    PD::setCursor(4, 87);
                    PD::print("Player 1: ");
                    break;

                case KeyboardMode::PlayerTwoName:
                    PD::setCursor(4, 87);
                    PD::print("Player 2: ");
                    break;

            }

            PD::setColor(8, 0);
            keyboardIndex = 2;
            break;


    }

    KeyboardSequenceVariables &keyboardSequenceVariables = this->state.getKeyboardSequenceVariables(keyboardIndex);


    // Render any existing message ..

    switch (mode) {

        case KeyboardMode::Message:

            messageLength = 21;
 
            for (uint8_t i = 0; i < keyboardSequenceVariables.messageIdx; i++) {

                this->doPlay_Keyboard_RenderKey(keyboardSequenceVariables.message[i], 25 + (i * 8));

            }

            if (PC::frameCount % 16 < 8) {
                    
                PD::drawFastVLine(26 + (keyboardSequenceVariables.messageIdx * 8), 87, 6);

            }

            break;

        case KeyboardMode::PlayerOneName:
        case KeyboardMode::PlayerTwoName:

            messageLength = 8;
 
            for (uint8_t i = 0; i < keyboardSequenceVariables.messageIdx; i++) {

                this->doPlay_Keyboard_RenderKey(keyboardSequenceVariables.message[i], 93 + (i * 8));

            }

            if (PC::frameCount % 16 < 8) {
                    
                PD::drawFastVLine(94 + (keyboardSequenceVariables.messageIdx * 8), 87, 6);

            }

            break;

    }

    if (keyboardSequenceVariables.highlightCount > 0) {

        keyboardSequenceVariables.highlightCount--;

        switch (keyboardSequenceVariables.y) {

            case 0 ... 2:

                PD::drawBitmap(keyboardPosX[keyboardSequenceVariables.x + (keyboardSequenceVariables.y * 10)], keyboardPosY[keyboardSequenceVariables.x + (keyboardSequenceVariables.y * 10)], Images::Keyboard_Highlight);
                break;

            case 3:

                switch (keyboardSequenceVariables.x) {

                    case 0 ... 1:
                        PD::drawBitmap(keyboardPosX[keyboardSequenceVariables.x + (keyboardSequenceVariables.y * 10)], keyboardPosY[keyboardSequenceVariables.x + (keyboardSequenceVariables.y * 10)], Images::Keyboard_Highlight);
                        break;

                    case 2 ... 7:
                        PD::drawBitmap(keyboardPosX[keyboardSequenceVariables.x + (keyboardSequenceVariables.y * 10)], keyboardPosY[keyboardSequenceVariables.x + (keyboardSequenceVariables.y * 10)], Images::Keyboard_Space);
                        break;


                }

                break;

        }

    }


    PD::drawBitmap(5, 102, Images::Keyboard_Full);
    PD::drawBitmap(keyboardPosX[keyboardSequenceVariables.x + (keyboardSequenceVariables.y * 10)], keyboardPosY[keyboardSequenceVariables.x + (keyboardSequenceVariables.y * 10)], Images::Keyboard_Outlines[keyboardHighlight[keyboardSequenceVariables.x + (keyboardSequenceVariables.y * 10)]]);

    if (PC::buttons.pressed(BTN_LEFT) || PC::buttons.repeat(BTN_LEFT, 4)) {

        switch (keyboardSequenceVariables.y) {

            case 0 ... 2:

                if (keyboardSequenceVariables.x > 0) {

                    keyboardSequenceVariables.x--;

                }
                break;

            case 3:

                switch (keyboardSequenceVariables.x) {

                    case 1:
                        keyboardSequenceVariables.x--;
                        break;

                    case 2 ... 7:
                        keyboardSequenceVariables.x = 1;
                        break;

                    case 8 ... 9:
                        keyboardSequenceVariables.x = 5;
                        break;

                }

                break;

        }

    }

    if (PC::buttons.pressed(BTN_RIGHT) || PC::buttons.repeat(BTN_RIGHT, 4)) {

        switch (keyboardSequenceVariables.y) {

            case 0 ... 2:

                if (keyboardSequenceVariables.x < 9) {

                    keyboardSequenceVariables.x++;

                } 
                break;

            case 3:

                switch (keyboardSequenceVariables.x) {

                    case 0 ... 1:
                        keyboardSequenceVariables.x++;
                        break;

                    case 2 ... 7:
                        keyboardSequenceVariables.x = 8;
                        break;

                }

                break;
        

        }

    }

    if (PC::buttons.pressed(BTN_UP) || PC::buttons.repeat(BTN_UP, 4)) {

        switch (keyboardSequenceVariables.y) {

            case 1 ... 2:
                keyboardSequenceVariables.y--;
                break;

            case 3:

                switch (keyboardSequenceVariables.x) {

                    case 0:
                        keyboardSequenceVariables.y--;
                        keyboardSequenceVariables.x = 0;
                        break;

                    case 1:
                        keyboardSequenceVariables.y--;
                        keyboardSequenceVariables.x = 0;
                        break;

                    case 2 ... 7:
                        keyboardSequenceVariables.y--;
                        break;

                    case 8 ... 9:
                        keyboardSequenceVariables.y--;
                        keyboardSequenceVariables.x = 7;
                        break;


                }

                break;
            
        }

    }

    if (PC::buttons.pressed(BTN_DOWN) || PC::buttons.repeat(BTN_DOWN, 4)) {

        switch (keyboardSequenceVariables.y) {

            case 0:

                keyboardSequenceVariables.y++;
                break;

            case 1:

                switch (keyboardSequenceVariables.x) {

                    case 0 ... 8:
                        keyboardSequenceVariables.y++;
                        break;

                    case 9:
                        keyboardSequenceVariables.y = 3;
                        keyboardSequenceVariables.x = 9;
                        break;

                }
                break;  

            case 2:

                switch (keyboardSequenceVariables.x) {

                    case 0:
                        keyboardSequenceVariables.y++;
                        keyboardSequenceVariables.x = 1;
                        break;

                    case 1:
                        keyboardSequenceVariables.y++;
                        keyboardSequenceVariables.x = 2;
                        break;

                    case 2 ... 6:
                        keyboardSequenceVariables.y++;
                        break;

                    case 7 ... 8:
                        keyboardSequenceVariables.y++;
                        keyboardSequenceVariables.x = 8;
                        break;

                }

                break;


        }

    }

    if (PC::buttons.pressed(BTN_A)) {

        uint8_t letterIdx = keyboardLetterIdx[keyboardSequenceVariables.x + (keyboardSequenceVariables.y * 10)];

        switch (letterIdx) {

            case KEY_START ... KEY_END:  // Letters

                if (keyboardSequenceVariables.messageIdx < messageLength) {

                    keyboardSequenceVariables.message[keyboardSequenceVariables.messageIdx] = ASCII_START + letterIdx;
                    keyboardSequenceVariables.messageIdx++;
                    keyboardSequenceVariables.highlightIdx = keyboardSequenceVariables.x + (keyboardSequenceVariables.y * 10);
                    keyboardSequenceVariables.highlightCount = 8;

                }
                break;

            case KEY_SMILEY_FACE:
            case KEY_FROWN_FACE:

                if (mode == KeyboardMode::Message && keyboardSequenceVariables.messageIdx < messageLength) {

                    keyboardSequenceVariables.message[keyboardSequenceVariables.messageIdx] = letterIdx;
                    keyboardSequenceVariables.messageIdx++;
                    keyboardSequenceVariables.highlightIdx = keyboardSequenceVariables.x + (keyboardSequenceVariables.y * 10);
                    keyboardSequenceVariables.highlightCount = 8;
                
                }
                break;

            case KEY_DELETE:  

                if (keyboardSequenceVariables.messageIdx > 0) {

                    keyboardSequenceVariables.messageIdx--;
                    keyboardSequenceVariables.message[keyboardSequenceVariables.messageIdx] = 0;

                }

                break;

            case KEY_SPACE:  // Space

                if (keyboardSequenceVariables.messageIdx > 0 && keyboardSequenceVariables.messageIdx < messageLength) {

                    keyboardSequenceVariables.message[keyboardSequenceVariables.messageIdx] = 32;
                    keyboardSequenceVariables.messageIdx++;
                    keyboardSequenceVariables.highlightIdx = keyboardSequenceVariables.x + (keyboardSequenceVariables.y * 10);
                    keyboardSequenceVariables.highlightCount = 8;

                }

                break;

            case KEY_PERIOD:  // Space

                if (keyboardSequenceVariables.messageIdx < messageLength) {

                    keyboardSequenceVariables.message[keyboardSequenceVariables.messageIdx] = ASCII_PERIOD;
                    keyboardSequenceVariables.messageIdx++;
                    keyboardSequenceVariables.highlightIdx = keyboardSequenceVariables.x + (keyboardSequenceVariables.y * 10);
                    keyboardSequenceVariables.highlightCount = 8;

                }

                break;

            case KEY_ENTER:

                this->doPlay_Keyboard_Exit(mode, keyboardSequenceVariables);
                break;

        }

    }

    if (PC::buttons.pressed(BTN_C)) {

        this->doPlay_Keyboard_Exit(mode, keyboardSequenceVariables);

    }

    if (PC::buttons.pressed(BTN_B)) {

        switch (mode) {

            case KeyboardMode::Message:

                keyboardSequenceVariables.messageIdx = 0;
                this->doPlay_Keyboard_Render();
                this->state.setGameState(GameState::Play_Hand);
                break;

            default: break;

        }

    }

}

void Game::doPlay_Keyboard_Render() {

    uint8_t currentPlayer = this->state.getCurrentTurn();

    this->renderBackground(this->getBackgroundColourByTurn(), 0, 0, 220, 178);
    this->renderLowerHand(currentPlayer);
    this->renderTopHand(currentPlayer == PLAYER_ONE ? PLAYER_TWO : PLAYER_ONE);
    this->renderTable(false);
    this->renderDeck();
    this->renderPlayerName(currentPlayer);
    this->renderScore(currentPlayer);
    this->renderPointerHand(currentPlayer, PointerPosition::Hand);

    PD::update();

}

void Game::doPlay_Keyboard_Exit(KeyboardMode mode, KeyboardSequenceVariables &keyboardSequenceVariables) {

    switch (mode) {

        case KeyboardMode::Message:
            keyboardSequenceVariables.counter = 0;
            this->doPlay_Keyboard_Render();
            this->state.setGameState(GameState::Play_Hand);
            break;

        case KeyboardMode::PlayerOneName:

            if (keyboardSequenceVariables.messageIdx > 0) {

                for (uint8_t i = 0; i < 8; i++) {

                    if (i < keyboardSequenceVariables.messageIdx) {
                        this->cookie->setPlayerChar(0, i, keyboardSequenceVariables.message[i]);
                    }
                    else {
                        this->cookie->setPlayerChar(0, i, 32);
                    }

                }

                this->cookie->saveCookie();
                this->state.setGameState(GameState::Play_PlayerTwoName_Init);

            }
            break;

        case KeyboardMode::PlayerTwoName:

            if (keyboardSequenceVariables.messageIdx > 0) {

                for (uint8_t i = 0; i < 8; i++) {

                    if (i < keyboardSequenceVariables.messageIdx) {
                        this->cookie->setPlayerChar(1, i, keyboardSequenceVariables.message[i]);
                    }
                    else {
                        this->cookie->setPlayerChar(1, i, 32);
                    }

                }

                this->cookie->saveCookie();
                this->state.setGameState(GameState::Play_CutDeck_Init);

            }
            break;

    }

}