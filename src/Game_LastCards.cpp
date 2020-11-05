#include "Game.h"
#include "Pokitto.h"
#include "images/images.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;


void Game::doPlay_LastCards_RenderPanel(uint8_t player) {

    uint8_t len = this->cookie->getPlayerLength(player);
    uint8_t j = 0;

    PD::drawBitmap(30, 30, Images::Panel);

    char name[] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 0 };
    for (uint8_t i = 0; i < 8; i++) {

        name[i] = this->cookie->getPlayerChar(player, i);

        if (name[i] != 32) j = i;

    }

    j = j + 2;
    name[j++] = 'W';
    name[j++] = 'I';
    name[j++] = 'N';
    name[j++] = 'S';
    name[j++] = ' ';
    name[j++] = 'C';
    name[j++] = 'A';
    name[j++] = 'R';
    name[j++] = 'D';
    name[j++] = 'S';

    this->renderPanelMessage(name, -4 + ((8 - (len - 1)) * 8 / 2), 0);

}

void Game::doPlay_LastCards_Init() { 

    switch (this->state.getGameMode()) {

        case GameMode::SinglePlayer:

            if (this->state.getLastScore() == PLAYER_ONE) {
                this->renderPanel(PanelMessage::LastCards_Human, 20);
            }
            else {
                this->renderPanel(PanelMessage::LastCards_Computer, 20);
            }                
            break;

        case GameMode::PlayAndPass:

            this->doPlay_LastCards_RenderPanel(this->state.getLastScore());
            break;

    }

    uint8_t numberOfCardsOnTable = this->state.numberOfCardsOnTable();

    #ifdef DEBUG
    uint8_t playerOne = 0;
    uint8_t playerTwo = 0;

    for (uint8_t i = 0; i < 40; i++) {

        switch (this->state.getDeckCard(i)) {

            case CardStatus::PlayerOneHand:
                playerOne++;
                break;

            case CardStatus::PlayerTwoHand:
                playerTwo++;
                break;
                
        }

    }

    printf("Before: Number of cards P1 %i, P2 %i, Deck %i\n", playerOne, playerTwo, this->state.numberOfCardsOnTable()); 
    #endif

    for (uint8_t i = 0; i < numberOfCardsOnTable; i++) {
        this->state.removeTableCard(this->state.getLastScore(), 0, true);
    }

    #ifdef DEBUG
    playerOne = 0;
    playerTwo = 0;

    for (uint8_t i = 0; i < 40; i++) {

        switch (this->state.getDeckCard(i)) {

            case CardStatus::PlayerOneHand:
                playerOne++;
                break;

            case CardStatus::PlayerTwoHand:
                playerTwo++;
                break;
                
        }

    }

    printf("After: Number of cards P1 %i, P2 %i, Deck %i\n", playerOne, playerTwo, this->state.numberOfCardsOnTable()); 
    #endif

    this->state.setGameState(GameState::Play_LastCards);

}

// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void Game::doPlay_LastCards() { 

    if (PC::buttons.pressed(BTN_A)) {

        this->state.setGameState(GameState::Play_Score_Init);
        
    }

}
