#include "Game.h"
#include "Pokitto.h"
#include "images/images.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;

void Game::calculateScores() {

    this->scoreSequenceVariables.reset();


    // Carte

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

    #ifdef DEBUG
    printf("Carte %i %i\n", playerOne, playerTwo);
    #endif

    this->scoreSequenceVariables.playerOne_Carte = (playerOne > playerTwo ? 1 : 0);
    this->scoreSequenceVariables.playerTwo_Carte = (playerTwo > playerOne ? 1 : 0);


    // Denari

    playerOne = 0;
    playerTwo = 0;

    for (uint8_t i = 0; i < 10; i++) {

        switch (this->state.getDeckCard(i)) {

            case CardStatus::PlayerOneHand:
                playerOne++;
                break;

            case CardStatus::PlayerTwoHand:
                playerTwo++;
                break;
                
        }

    }

    #ifdef DEBUG
    printf("Denari %i %i\n", playerOne, playerTwo);
    #endif
    
    this->scoreSequenceVariables.playerOne_Denari = (playerOne > playerTwo ? 1 : 0);
    this->scoreSequenceVariables.playerTwo_Denari = (playerTwo > playerOne ? 1 : 0);


    // Sette Bello

    playerOne = 0;
    playerTwo = 0;

    switch (this->state.getDeckCard(6)) {

        case CardStatus::PlayerOneHand:
            playerOne++;
            break;

        case CardStatus::PlayerTwoHand:
            playerTwo++;
            break;
            
    }
    
    #ifdef DEBUG
    printf("Sette Bello %i %i\n", playerOne, playerTwo);
    #endif

    this->scoreSequenceVariables.playerOne_SetteBello = (playerOne > playerTwo ? 1 : 0);
    this->scoreSequenceVariables.playerTwo_SetteBello = (playerTwo > playerOne ? 1 : 0);


    // Premiera

    playerOne = 0;
    playerTwo = 0;

    uint8_t playerOne_Suits[4] = { 0, 0, 0, 0};
    uint8_t playerTwo_Suits[4] = { 0, 0, 0, 0};

    for (uint8_t i = 0; i < 40; i++) {

        uint8_t suit = i / 10;
        uint8_t card = (i % 10) + 1;

        switch (this->state.getDeckCard(i)) {

            case CardStatus::PlayerOneHand:
                if (playerOne_Suits[suit] < card) playerOne_Suits[suit] = card;
                break;

            case CardStatus::PlayerTwoHand:
                if (playerTwo_Suits[suit] < card) playerTwo_Suits[suit] = card;
                break;
                
        }

    }

    const uint8_t cardValue[11] = {0, 16, 12, 13, 14, 15, 18, 21, 10, 10, 10 };

    for (uint8_t i= 0; i < 4; i++) {

        if (playerOne_Suits[i] > 0) playerOne = playerOne + cardValue[playerOne_Suits[i]];
        if (playerTwo_Suits[i] > 0) playerTwo = playerTwo + cardValue[playerTwo_Suits[i]];

    }

    #ifdef DEBUG
    printf("Premiera %i %i\n", playerOne, playerTwo);
    #endif

    this->scoreSequenceVariables.playerOne_Premiera = (playerOne > playerTwo ? 1 : 0);
    this->scoreSequenceVariables.playerTwo_Premiera = (playerTwo > playerOne ? 1 : 0);

    #ifdef DEBUG
    printf("PlayerOne Score %i %i %i\n", this->state.getScore(PLAYER_ONE), this->state.getScopa(PLAYER_ONE), this->scoreSequenceVariables.playerOne_Score());
    printf("PlayerTwo Score %i %i %i\n", this->state.getScore(PLAYER_TWO), this->state.getScopa(PLAYER_TWO), this->scoreSequenceVariables.playerTwo_Score());
    #endif

//SJH 11
    if (this->state.getScore(PLAYER_ONE) + this->state.getScopa(PLAYER_ONE) + this->scoreSequenceVariables.playerOne_Score() >= 11 || this->state.getScore(PLAYER_TWO) + this->state.getScopa(PLAYER_TWO) + this->scoreSequenceVariables.playerTwo_Score() >= 11) {
//    if (this->state.getScore(PLAYER_ONE) + this->state.getScopa(PLAYER_ONE) + this->scoreSequenceVariables.playerOne_Score() >= 1 || this->state.getScore(PLAYER_TWO) + this->state.getScopa(PLAYER_TWO) + this->scoreSequenceVariables.playerTwo_Score() >= 1) {

        if (this->state.getScore(PLAYER_ONE) + this->state.getScopa(PLAYER_ONE) + this->scoreSequenceVariables.playerOne_Score() > this->state.getScore(PLAYER_TWO) + this->state.getScopa(PLAYER_TWO) + this->scoreSequenceVariables.playerTwo_Score()) {

            this->scoreSequenceVariables.winner = PLAYER_ONE;
            this->scoreSequenceVariables.counter = GAME_OVER_DELAY;
            
        }
        else if (this->state.getScore(PLAYER_ONE) + this->state.getScopa(PLAYER_ONE) + this->scoreSequenceVariables.playerOne_Score() < this->state.getScore(PLAYER_TWO) + this->state.getScopa(PLAYER_TWO) + this->scoreSequenceVariables.playerTwo_Score()) {

            this->scoreSequenceVariables.winner = PLAYER_TWO;
            this->scoreSequenceVariables.counter = GAME_OVER_DELAY;
            
        }
        else {

            this->scoreSequenceVariables.winner = DRAW;
            this->scoreSequenceVariables.counter = GAME_OVER_DELAY;

        }

    }
    else {

        this->scoreSequenceVariables.winner = NO_WINNER;
        this->scoreSequenceVariables.counter = GAME_OVER_DELAY;

    }

}

void Game::doPlay_Score_Init() { 


    this->state.setGameState(GameState::Play_Score);
    this->calculateScores();

    uint32_t startPos = (SDCardLength * 40) + 4 + (220 * 176 / 2);
    file.seek(SDScoreStart + 2);
    file.read(PD::getBuffer(), SDScoreLength - 2);
    PD::update();

}


void Game::doPlay_Score() { 

    PD::setColor(7, 14);

    bool flashOne = this->scoreSequenceVariables.winner == PLAYER_ONE || this->scoreSequenceVariables.winner == DRAW;
    bool flashTwo = this->scoreSequenceVariables.winner == PLAYER_TWO || this->scoreSequenceVariables.winner == DRAW;

    this->doPlay_Score_Line(0, this->state.getScore(PLAYER_ONE), this->state.getScore(PLAYER_TWO), false, false);
    this->doPlay_Score_Line(1, this->state.getScopa(PLAYER_ONE), this->state.getScopa(PLAYER_TWO), false, false);
    this->doPlay_Score_Line(2, this->scoreSequenceVariables.playerOne_Carte, this->scoreSequenceVariables.playerTwo_Carte, false, false);
    this->doPlay_Score_Line(3, this->scoreSequenceVariables.playerOne_Denari, this->scoreSequenceVariables.playerTwo_Denari, false, false);
    this->doPlay_Score_Line(4, this->scoreSequenceVariables.playerOne_SetteBello, this->scoreSequenceVariables.playerTwo_SetteBello, false, false);
    this->doPlay_Score_Line(5, this->scoreSequenceVariables.playerOne_Premiera, this->scoreSequenceVariables.playerTwo_Premiera, false, false);
    this->doPlay_Score_Line(6, this->state.getScore(PLAYER_ONE) + this->state.getScopa(PLAYER_ONE) + this->scoreSequenceVariables.playerOne_Score(), 
                               this->state.getScore(PLAYER_TWO) + this->state.getScopa(PLAYER_TWO) + this->scoreSequenceVariables.playerTwo_Score(), flashOne, flashTwo);

    if (PC::buttons.pressed(BTN_A)) {

        if (this->scoreSequenceVariables.winner == NO_WINNER) {

            this->state.setScore(PLAYER_ONE, this->state.getScore(PLAYER_ONE) + this->state.getScopa(PLAYER_ONE) + this->scoreSequenceVariables.playerOne_Score());
            this->state.setScore(PLAYER_TWO, this->state.getScore(PLAYER_TWO) + this->state.getScopa(PLAYER_TWO) + this->scoreSequenceVariables.playerTwo_Score());
            this->state.swapDealer();
            this->state.shuffle(false);
            this->state.setGameState(GameState::Play_Deal_Init);
            this->renderBackground(this->getBackgroundColourByDeal());
            PD::update();
            return;

        }
        else {

            this->state.setGameState(GameState::Title_Init);

        }

    }

    if (this->scoreSequenceVariables.counter > 0) {

        this->scoreSequenceVariables.counter--;

    }
    else {

        switch (this->scoreSequenceVariables.winner) {

            case PLAYER_ONE:

                switch (this->state.getGameMode()) {

                    case GameMode::SinglePlayer:
                        this->renderPanel(PanelMessage::HumanWins);
                        break;

                    case GameMode::PlayAndPass:
                        this->doPlay_Score_RenderPanel(PLAYER_ONE);
                        break;

                }
                break;

            case PLAYER_TWO:

                switch (this->state.getGameMode()) {

                    case GameMode::SinglePlayer:
                        this->renderPanel(PanelMessage::PlayerComputerWins);
                        break;

                    case GameMode::PlayAndPass:
                        this->doPlay_Score_RenderPanel(PLAYER_TWO);
                        break;

                }
                break;

            case DRAW:
                this->renderPanel(PanelMessage::Draw);
                break;

            case NO_WINNER:
                PD::drawBitmap(64, 162, Images::PressAButton);
                break;
                
        }        

    }

}

void Game::doPlay_Score_RenderPanel(uint8_t player) {

    uint8_t len = this->cookie->getPlayerLength(player);
    PD::drawBitmap(30, 30, Images::Panel);

    char name[] = { 'Y', 'O', 'U', ' ', 'W', 'O', 'N', ' ', '1', '2', '3', '4', '5', '6', '7', '8', 0 };
    for (uint8_t i = 0; i < 8; i++) {
        name[8 + i] = this->cookie->getPlayerChar(player, i);
    }

    this->renderPanelMessage(name, 8 + (8 - (len - 1)) * 8 / 2, 0);

}

void Game::doPlay_Score_Line(uint8_t y, uint8_t score1, uint8_t score2, bool flashScore1, bool flashScore2) { 

    constexpr uint8_t xColumns[] = { 146, 176 };
    constexpr uint8_t yColumns[] = { 48, 68, 80, 92, 104, 116, 136 };

    PD::setColor(7, 14);

    if (!flashScore1 || (PC::frameCount % 16) < 8) {

        if (score1 < 10) {
            PD::setCursor(xColumns[0], yColumns[y]);
            PD::print(score1, 10);
        }
        else {
            PD::setCursor(xColumns[0] - 6, yColumns[y]);
            PD::print(score1, 10);
        }

    }

    if (!flashScore2 || (PC::frameCount % 16) < 8) {

        if (score2 < 10) {
            PD::setCursor(xColumns[1], yColumns[y]);
            PD::print(score2, 10);
        }
        else {
            PD::setCursor(xColumns[1] - 6, yColumns[y]);
            PD::print(score2, 10);
        }

    }

}
