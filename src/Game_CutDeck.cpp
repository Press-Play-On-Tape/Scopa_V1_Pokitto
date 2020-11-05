#include "Game.h"
#include "Pokitto.h"
#include "images/images.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;

void Game::doPlay_CutDeck_Init() { 

    this->state.shuffle(true);
    this->state.setGameState(GameState::Play_CutDeck);

    this->cutDeckSequenceVariables.reset();
    this->state.reset();

}

void Game::doPlay_CutDeck() { 

    if (this->cutDeckSequenceVariables.waitForSwap) {

        this->renderBackground(this->getBackgroundColourByDeal());
        this->doPlay_CutDeck_RenderCards();

        switch (this->state.getGameMode()) {

            case GameMode::SinglePlayer:
                this->renderPanel(PanelMessage::SwapPlayers, -20);
                break;

            case GameMode::PlayAndPass:
                this->doPlay_SwapPlayers_RenderPanel(PLAYER_ONE, -20);
                break;

        }

        if (PC::buttons.pressed(BTN_A)) {

            Card p1 = this->state.getCardFromDeck(this->state.getSelectedCard(PLAYER_ONE));
            Card p2 = this->state.getCardFromDeck(this->state.getSelectedCard(PLAYER_TWO));

            #ifdef DEBUG_PLAYER_ONE_PLAYS_FIRST
            p1.setNumber(10);
            p2.setNumber(2);
            #endif

            #ifdef DEBUG_PLAYER_TWO_PLAYS_FIRST
            p1.setNumber(3);
            p2.setNumber(9);
            #endif

            this->state.shuffle(false);
            this->state.setCurrentDeal(p1.getNumber() > p2.getNumber() ? PLAYER_ONE : PLAYER_TWO);
            this->state.setGameState(GameState::Play_Deal_Init);
            this->renderBackground(this->getBackgroundColourByDeal());
            return;

        }

        return;

    }

    if (this->cutDeckSequenceVariables.counter > 0) {
    
        this->cutDeckSequenceVariables.counter--;
    
        if (this->cutDeckSequenceVariables.counter == 0) {

            if (this->state.getSelectedCard(PLAYER_TWO) != 255) {

                Card p1 = this->state.getCardFromDeck(this->state.getSelectedCard(PLAYER_ONE));
                Card p2 = this->state.getCardFromDeck(this->state.getSelectedCard(PLAYER_TWO));

                #ifdef DEBUG_PLAYER_ONE_PLAYS_FIRST
                p1.setNumber(10);
                p2.setNumber(2);
                #endif

                #ifdef DEBUG_PLAYER_TWO_PLAYS_FIRST
                p1.setNumber(3);
                p2.setNumber(9);
                #endif

                if (p1.getNumber() > p2.getNumber() && this->state.getCurrentTurn() == PLAYER_TWO) {

                    switch (this->state.getGameMode()) {

                        case GameMode::SinglePlayer:
                            this->state.shuffle(false);
                            this->state.setCurrentDeal(p1.getNumber() > p2.getNumber() ? PLAYER_ONE : PLAYER_TWO);
                            this->state.setGameState(GameState::Play_Deal_Init);
                            this->renderBackground(this->getBackgroundColourByDeal());
                            return;

                        case GameMode::PlayAndPass:
                            this->cutDeckSequenceVariables.waitForSwap = true;
                            return;

                    }

                }
                else {

                    this->state.shuffle(false);
                    this->state.setCurrentDeal(p1.getNumber() > p2.getNumber() ? PLAYER_ONE : PLAYER_TWO);
                    this->state.setGameState(GameState::Play_Deal_Init);
                    this->renderBackground(this->getBackgroundColourByDeal());
                    return;

                }

            }
            else {

                this->state.swapPlayers();
                return;

            }

        }

    }
    else {

        if (this->state.getCurrentTurn() == PLAYER_ONE) {

            this->renderBackground(COLOUR_P1);    
            PD::setColor(7, COLOUR_P1);
            
            switch (this->state.getGameMode()) {

                case GameMode::SinglePlayer:
                    this->renderPanel(PanelMessage::PickACard_Human, -20);
                    break;

                case GameMode::PlayAndPass:
                    this->doPlay_CutDeck_RenderPanel(PLAYER_ONE);
                    break;

            }

        }
        else {

            this->renderBackground(this->getBackgroundColourByTurn());    
            PD::setColor(7, COLOUR_P2);
            
            switch (this->state.getGameMode()) {

                case GameMode::SinglePlayer:
                    this->renderPanel(PanelMessage::PickACard_Computer, -20);
                    break;

                case GameMode::PlayAndPass:
                    this->doPlay_CutDeck_RenderPanel(PLAYER_TWO);
                    break;

            }

        }

        this->doPlay_CutDeck_RenderCards();

        if (this->cutDeckSequenceVariables.counter == 0) {
            PD::drawBitmap(56 + (this->cutDeckSequenceVariables.cursor * 14), 120, Images::Hand);
        }

        if (this->state.getCurrentTurn() == PLAYER_ONE) {

            if (PC::buttons.pressed(BTN_LEFT) && this->cutDeckSequenceVariables.cursor > 0) {

                if (this->cutDeckSequenceVariables.counter == 0) {

                    this->cutDeckSequenceVariables.cursor--;

                }
                else {

                    this->cutDeckSequenceVariables.counter = 1;

                }

            }

            if (PC::buttons.pressed(BTN_RIGHT) && this->cutDeckSequenceVariables.cursor < 9) {

                if (this->cutDeckSequenceVariables.counter == 0) {

                    this->cutDeckSequenceVariables.cursor++;

                }
                else {

                    this->cutDeckSequenceVariables.counter = 1;
                    
                }

            }

            if (this->cutDeckSequenceVariables.counter == 0) {

                if (PC::buttons.pressed(BTN_A)) {

                    this->state.setSelectedCard(PLAYER_ONE, this->cutDeckSequenceVariables.cursor);
                    this->cutDeckSequenceVariables.cursor = (this->cutDeckSequenceVariables.cursor + 1) % 10;
                    this->cutDeckSequenceVariables.counter = (this->state.getGameMode() == GameMode::SinglePlayer ? 64 : 8);

                    switch (this->state.getGameMode()) {

                        case GameMode::SinglePlayer:
                            this->renderBackground(COLOUR_P1, 0, 93, 220, 60);
                            break;

                        case GameMode::PlayAndPass:
                            this->renderBackground(this->state.getCurrentDeal() == PLAYER_ONE ? COLOUR_P2 : COLOUR_P1, 0, 93, 220, 60);
                            break;

                    }

                    this->doPlay_CutDeck_RenderCards();

                }

            }

        }

        else if (this->state.getCurrentTurn() == PLAYER_TWO) {

            switch (this->state.getGameMode()) {

                case GameMode::SinglePlayer:

                    // Select a card ..

                    this->cutDeckSequenceVariables.cursor = this->state.getSelectedCard(PLAYER_ONE);
                    while (this->cutDeckSequenceVariables.cursor == this->state.getSelectedCard(PLAYER_ONE)) {
                        this->cutDeckSequenceVariables.cursor = random(0, 9);
                    }

                    this->state.setSelectedCard(PLAYER_TWO, this->cutDeckSequenceVariables.cursor);
                    this->cutDeckSequenceVariables.counter = 48;

                    switch (this->state.getGameMode()) {

                        case GameMode::SinglePlayer:
                            this->renderBackground(COLOUR_P1, 0, 93, 220, 60);
                            break;

                        case GameMode::PlayAndPass:
                            this->renderBackground(this->state.getCurrentDeal() == PLAYER_ONE ? COLOUR_P2 : COLOUR_P1, 0, 93, 220, 60);
                            break;

                    }
                    
                    this->doPlay_CutDeck_RenderCards();

                    break;

                case GameMode::PlayAndPass:
                                    
                    if (PC::buttons.pressed(BTN_LEFT)) {

                        if (this->cutDeckSequenceVariables.counter == 0) {
                        
                            if (this->cutDeckSequenceVariables.cursor > 0) {

                                if (this->state.getSelectedCard(PLAYER_ONE) != this->cutDeckSequenceVariables.cursor - 1) {
                                    this->cutDeckSequenceVariables.cursor--;
                                }
                                else if (this->cutDeckSequenceVariables.cursor > 1) {
                                    this->cutDeckSequenceVariables.cursor = this->cutDeckSequenceVariables.cursor - 2;
                                }

                            }

                        }
                        else {

                            this->cutDeckSequenceVariables.counter = 1;
                            
                        }

                    }

                    if (PC::buttons.pressed(BTN_RIGHT)) {

                        if (this->cutDeckSequenceVariables.counter == 0) {

                            if (this->cutDeckSequenceVariables.cursor < 9) {

                                if (this->state.getSelectedCard(PLAYER_ONE) != this->cutDeckSequenceVariables.cursor + 1) {
                                    this->cutDeckSequenceVariables.cursor++;
                                }
                                else if (this->cutDeckSequenceVariables.cursor < 8) {
                                    this->cutDeckSequenceVariables.cursor = this->cutDeckSequenceVariables.cursor + 2;
                                }

                            }

                        }
                        else {

                            this->cutDeckSequenceVariables.counter = 1;
                            
                        }


                    }

                    if (this->cutDeckSequenceVariables.counter == 0) {

                        if (PC::buttons.pressed(BTN_A)) {

                            this->state.setSelectedCard(PLAYER_TWO, this->cutDeckSequenceVariables.cursor);
                            this->cutDeckSequenceVariables.counter = 16;
                            this->doPlay_CutDeck_RenderCards();

                        }

                    }

                    break;

            }

        }

    }

}

void Game::doPlay_CutDeck_RenderPanel(uint8_t player) {

    uint8_t len = this->cookie->getPlayerLength(player);
    PD::drawBitmap(30, 10, Images::Panel);

    char name[] = { 'Y', 'O', 'U', 'R', ' ', 'P', 'I', 'C', 'K', ' ', '1', '2', '3', '4', '5', '6', '7', '8', 0 };
    for (uint8_t i = 0; i < 8; i++) {
        name[10 + i] = this->cookie->getPlayerChar(player, i);
    }

    this->renderPanelMessage(name, (8 - (len - 1)) * 8 / 2, -20);

}

void Game::doPlay_CutDeck_RenderCards() {

    PD::drawBitmap(2, 93, Images::Card_Small_Deck_04);

    for (uint8_t x = 0; x < 10; x++) {

        if (this->state.getSelectedCard(PLAYER_ONE) == x) {

            Card card = this->state.getCardFromDeck(x);

            if (this->state.getSelectedCard(PLAYER_TWO) != 255) {

                Card otherCard = this->state.getCardFromDeck(this->state.getSelectedCard(PLAYER_TWO));

                if (otherCard.getNumber() < card.getNumber() && (PC::frameCount / 8) % 2 == 0) {

                    this->renderSmallCard(52 + (x * 14), 80, card, true);

                }
                else {

                    this->renderSmallCard(52 + (x * 14), 80, card, false);

                }

            }
            else {

                this->renderSmallCard(52 + (x * 14), 80, card, false);

            }

        }
        else if (this->state.getSelectedCard(PLAYER_TWO) == x) {

            Card card = this->state.getCardFromDeck(x);

            if (this->state.getSelectedCard(PLAYER_ONE) != 255) {

                Card otherCard = this->state.getCardFromDeck(this->state.getSelectedCard(PLAYER_ONE));

                if (otherCard.getNumber() < card.getNumber() && (PC::frameCount / 8) % 2 == 0) {
                    this->renderSmallCard(52 + (x * 14) - (card.getNumber() == 9 ? 1 : 0), 110, card, true);

                }
                else {

                    this->renderSmallCard(52 + (x * 14) - (card.getNumber() == 9 ? 1 : 0), 110, card, false);

                }

            }
            else {

                this->renderSmallCard(52 + (x * 14) - (card.getNumber() == 9 ? 1 : 0), 110, card, false);

            }

        }
        else {

            PD::drawBitmap(52 + (x * 14), 95, Images::Card_Small_Back);

        }

    }

}