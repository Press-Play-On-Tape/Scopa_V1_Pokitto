
#include "Pokitto.h"
#include "Game.h"
#include "utils/Consts.h"
#include "images/Images.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;

void Game::doPlay_Hand_Init() {

    uint8_t currentPlayer = this->state.getCurrentTurn();

    switch (currentPlayer) {

        case PLAYER_ONE:
            this->state.setPointer(PLAYER_ONE, PointerPosition::Hand, 0);
            this->renderPointerHand(PLAYER_ONE, PointerPosition::Hand);
            break;

        case PLAYER_TWO:

            switch (this->state.getGameMode()) {

                case GameMode::SinglePlayer:
                    break;

                case GameMode::PlayAndPass:
                    this->state.setPointer(PLAYER_TWO, PointerPosition::Hand, 0);
                    this->renderPointerHand(PLAYER_TWO, PointerPosition::Hand);
                    break;

            }

            break;

    }

    this->state.setGameState(GameState::Play_Hand);
    this->debugHands();

}

void Game::doPlay_Hand() {


    uint8_t currentPlayer = this->state.getCurrentTurn();


    // Display messages ?

    KeyboardSequenceVariables &keyboardSequenceVariables = this->state.getKeyboardSequenceVariables(this->state.getCurrentTurn());

    if (keyboardSequenceVariables.messageIdx > 0) {

        if (keyboardSequenceVariables.counter == 0) {

            uint8_t left = 110 - ((keyboardSequenceVariables.messageIdx * 8) / 2);
            uint8_t right = 110 + ((keyboardSequenceVariables.messageIdx * 8) / 2);

            PD::drawBitmap(left - 8, 4, Images::TalkingBubbles[0]);
            for (uint8_t i = 0; i < keyboardSequenceVariables.messageIdx; i++) {
                PD::drawBitmap(left + (i * 8) - 2, 4, Images::TalkingBubbles[1]);
            }
            PD::drawBitmap(right - 2, 4, Images::TalkingBubbles[2]);

            PD::setColor(8, 7);

            this->renderString(keyboardSequenceVariables.message, keyboardSequenceVariables.messageIdx, left, 7);
            keyboardSequenceVariables.counter = 192;

        }
        else {

            keyboardSequenceVariables.counter--;

            if (keyboardSequenceVariables.counter == 0) {

                keyboardSequenceVariables.messageIdx = 0;
                this->renderBackground(this->getBackgroundColourByTurn(), 0, 0, 220, 31);
                this->renderTopHand(currentPlayer == PLAYER_ONE ? PLAYER_TWO : PLAYER_ONE);
                this->renderPlayerName(currentPlayer);
                this->renderScore(currentPlayer);
                PD::update();
                return;

            }

        }

    }    



    // Handle movements

    switch (currentPlayer) {

        case PLAYER_ONE:
            this->doPlay_Hand_HandleActions(keyboardSequenceVariables, currentPlayer);
            break;

        case PLAYER_TWO:

            switch (this->state.getGameMode()) {

                case GameMode::SinglePlayer:
                    this->doPlay_Hand_ComputerAction();
                    break;

                case GameMode::PlayAndPass:
                    this->doPlay_Hand_HandleActions(keyboardSequenceVariables, currentPlayer);
                    break;

            }

            break;
            
    }

}


void Game::doPlay_Hand_HandleActions(KeyboardSequenceVariables keyboardSequenceVariables, uint8_t currentPlayer) {

    if (PC::buttons.pressed(BTN_LEFT) && this->state.getPointer(currentPlayer, PointerPosition::Hand) > 0) {

        this->state.setPointer(currentPlayer, PointerPosition::Hand, this->state.getPointer(currentPlayer, PointerPosition::Hand) - 1);
        this->renderLowerHand(currentPlayer, true);
        this->renderPointerHand(currentPlayer, PointerPosition::Hand);
        PD::update();

    }

    if (PC::buttons.pressed(BTN_RIGHT) && this->state.getPointer(currentPlayer, PointerPosition::Hand) < this->state.numberOfCardsInHand(currentPlayer) - 1) {

        this->state.setPointer(currentPlayer, PointerPosition::Hand, this->state.getPointer(currentPlayer, PointerPosition::Hand) + 1);
        this->renderLowerHand(currentPlayer, true);
        this->renderPointerHand(currentPlayer, PointerPosition::Hand);
        PD::update();

    }

    switch (this->state.getGameMode()) {

        case GameMode::SinglePlayer:

            break;

        case GameMode::PlayAndPass:

            if (PC::buttons.pressed(BTN_B)) {

                keyboardSequenceVariables = this->state.getKeyboardSequenceVariables(currentPlayer == 0 ? 1 : 0);
                this->state.setGameState(GameState::Play_Keyboard_Init);

            }

            break;

    }

    if (PC::buttons.pressed(BTN_A) || PC::buttons.pressed(BTN_UP)) {

        this->state.setSelectedCard(currentPlayer);


        // Is the card a match for others?

        uint8_t numberOfCardsOnTable = this->state.numberOfCardsOnTable();
        uint8_t matches = 0;
        uint8_t matchIdx = 255;
        uint8_t selectedIdx = this->state.getSelectedCard(currentPlayer);

        Card selectedCard = this->state.getHandCard(currentPlayer, selectedIdx);

        for (uint8_t i = 0; i < numberOfCardsOnTable; i++) {

            if (this->state.getTableCard(i).getNumber() == selectedCard.getNumber()) {

                matches++;
                if (matchIdx == NO_SELECTION) matchIdx = i;

            }

        }

        if (matches == 1) {
            this->state.setSelectedTableCard(matchIdx, true);
            this->state.setGameState(GameState::Play_TakeMatch);
            this->renderTable(true);
            this->counter = 33;
            return;

        }
        else if (matches > 1) {

            this->state.setGameState(GameState::Play_NominateDeckCards_Single);
            this->state.setPointer(currentPlayer, PointerPosition::Table, matchIdx);
            this->renderPointerHand(currentPlayer, PointerPosition::Table);
            return;

        }


        // Is there a multi-card match?

        uint8_t selectedIndices[20] =   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
        uint8_t matchesArray[20] =      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };

        for (uint8_t x = 0; x < numberOfCardsOnTable; x++) {
            selectedIndices[x] = this->state.getTableCard(x).getNumber() + 1;
        }

        for (uint8_t i = 2; i <= numberOfCardsOnTable; i++) {
            Combi(selectedCard, matches, matchesArray, selectedIndices, i, 0, 0, this->check, numberOfCardsOnTable);
        }

        if (matches == 1) {

            for (uint8_t i = 0; i < 20; i++){

                if (i == 0 || (i > 0 && matchesArray[i] != 0)) {
                    this->state.setSelectedTableCard(matchesArray[i], true);
                }

            }

            this->state.setGameState(GameState::Play_TakeMatch);
            this->counter = 33;
            return;

        }
        else if (matches > 1) {

            this->state.setGameState(GameState::Play_NominateDeckCards_Multiple);
            this->state.setPointer(currentPlayer, PointerPosition::Table, 0);
            this->renderPointerHand(currentPlayer, PointerPosition::Table);
            return;

        }



        // If there are no matches then add the card to the table ..

        this->state.setGameState(GameState::Play_AddCardToHand);
        this->counter = 97;

    }

}



void Game::Combi(Card cardToMatch, uint8_t &matches, uint8_t matchArray[], uint8_t a[], uint8_t reqLen, uint8_t s, uint8_t currLen, bool check[], uint8_t l) {

    if (currLen > reqLen) return;

    if (currLen == reqLen) {

        uint8_t cardValues = 0;

        for (int i = 0; i < l; i++) {
            if (check[i] == true) {
                cardValues = cardValues + a[i];
            }

        }


    // Does this match the value of our card?

        if (cardValues == cardToMatch.getNumber() + 1) {

            uint8_t j = 0;
            
            for (int i = 0; i < l; i++) {
                if (check[i] == true) {
                    matchArray[j] = i;
                    j++;
                }

            }
            matches++;
        }

        return;

    }

    if (s == l) {
        return;
    }

    check[s] = true;
    Combi(cardToMatch, matches, matchArray, a, reqLen, s + 1, currLen + 1, check, l);
    check[s] = false;
    Combi(cardToMatch, matches, matchArray, a, reqLen, s + 1, currLen, check, l);

}

void Game::doPlay_TakeMatch_IsCoin(uint8_t currentPlayer, uint8_t selectedIdx) {

    if (this->state.getHandCard(currentPlayer, selectedIdx).getSuit() == SUIT_COINS) {

        this->playSoundEffect(SoundTheme::Coins);

    }
    else {

        for (uint8_t i = 0; i < this->state.numberOfCardsOnTable(); i++) {
            
            if (this->state.getSelectedTableCard(i)) {

                if (this->state.getTableCard(i).getSuit() == SUIT_COINS) {
                    this->playSoundEffect(SoundTheme::Coins);
                    break;
                }
                
            }

        }

    }

}

void Game::doPlay_TakeMatch() {

    uint8_t currentPlayer = this->state.getCurrentTurn();
    uint8_t selectedIdx = this->state.getSelectedCard(currentPlayer);

    if (this->counter > 0) {

        this->counter--;

        switch (this->state.getGameMode()) {

            case GameMode::SinglePlayer:

                switch (currentPlayer) {

                    case PLAYER_ONE:

                        switch (this->counter) {

                            case 32:
                                this->renderTable(false);
                                this->renderLowerHand(currentPlayer, false);
                                this->doPlay_TakeMatch_IsCoin(currentPlayer, selectedIdx);
                                break;

                            case 16:
                                this->renderTable(false);
                                this->renderLowerHand(currentPlayer, false);
                                break;

                            case 24:
                            case 8:
                                this->renderTable(true);
                                this->renderLowerHand(currentPlayer, true);
                                break;

                            case 0:
                                this->state.removeTableCards(currentPlayer, true);
                                this->state.removeHandCard(currentPlayer, selectedIdx, true);
                                this->state.clearSelectedTableCards();
                                this->state.clearSelectedCard(currentPlayer);
                                this->renderTable(false);
                                this->renderLowerHand(PLAYER_ONE, false, false);

                                if (this->state.numberOfCardsOnTable() == 0) {

                                    this->state.incScopa(currentPlayer);
                                    this->state.setGameState(GameState::Play_Scopa_Init);

                                }
                                else {

                                    this->state.setGameState(GameState::Play_SwapPlayers_Init);

                                }

                                break;

                        }

                        break;

                    case PLAYER_TWO:
 
                        switch (this->counter) {

                            case 64:
                                this->doPlay_TakeMatch_IsCoin(currentPlayer, selectedIdx);
                                this->renderTable(false);
                                this->renderTopHand(currentPlayer, true, false);
                                PD::update();
                                break;

                            case 32:
                                this->renderTable(false);
                                this->renderTopHand(currentPlayer, true, false);
                                PD::update();
                                break;

                            case 48:
                            case 16:
                                this->renderTable(true);
                                this->renderTopHand(currentPlayer, true, true);
                                PD::update();
                                break;

                            case 0:
                                this->state.removeTableCards(currentPlayer, true);
                                this->state.removeHandCard(currentPlayer, selectedIdx, true);
                                this->state.clearSelectedTableCards();
                                this->state.clearSelectedCard(currentPlayer);
                                this->renderTable(false);
                                this->renderTopHand(PLAYER_ONE, false, true);

                                if (this->state.numberOfCardsOnTable() == 0) {

                                    this->state.incScopa(currentPlayer);
                                    this->state.setGameState(GameState::Play_Scopa_Init);

                                }
                                else {

                                    this->state.setGameState(GameState::Play_SwapPlayers_Init);

                                }

                                PD::update();
                                break;

                        }

                        break;

                }                   

                break;

            case GameMode::PlayAndPass:

                switch (this->counter) {

                    case 32:
                        this->doPlay_TakeMatch_IsCoin(currentPlayer, selectedIdx);
                        this->renderTable(false);
                        this->renderLowerHand(currentPlayer, false);
                        break;

                    case 16:
                        this->renderTable(false);
                        this->renderLowerHand(currentPlayer, false);
                        break;

                    case 24:
                    case 8:
                        this->renderTable(true);
                        this->renderLowerHand(currentPlayer, true);
                        break;

                    case 0:
                        this->state.removeTableCards(currentPlayer, true);
                        this->state.removeHandCard(currentPlayer, selectedIdx, true);
                        this->state.clearSelectedTableCards();
                        this->state.clearSelectedCard(currentPlayer);
                        this->renderTable(false);
                        this->renderLowerHand(currentPlayer, false, true);

                        if (this->state.numberOfCardsOnTable() == 0) {

                            this->state.incScopa(currentPlayer);
                            this->state.setGameState(GameState::Play_Scopa_Init);

                        }
                        else {

                            this->state.setGameState(GameState::Play_SwapPlayers_Init);

                        }

                        break;

                }

                break;

        }

    }

}

void Game::doPlay_AddToTable() {

    uint8_t currentPlayer = this->state.getCurrentTurn();
    uint8_t selectedIdx = this->state.getSelectedCard(currentPlayer);

    if (this->counter > 0) {

        this->counter--;

        switch (this->state.getGameMode()) {

            case GameMode::SinglePlayer:

                switch (currentPlayer) {

                    case PLAYER_ONE:

                        switch (this->counter) {

                            case 96:
                            case 80:
                                this->renderLowerHand(currentPlayer, false);
                                break;

                            case 88:
                            case 72:
                                this->renderLowerHand(currentPlayer, true);
                                break;

                            case 64:
                                {
                                    Card selectedCard = this->state.getHandCard(currentPlayer, selectedIdx);
                                    this->state.removeHandCard(currentPlayer, selectedIdx, false);
                                    this->state.clearSelectedCard(currentPlayer);
                                    this->renderLowerHand(currentPlayer, false);
                                    this->state.addTableCard(selectedCard);
                                    this->state.setSelectedTableCard(this->state.numberOfCardsOnTable() - 1, true);
                                    this->renderTable(true);
                                }
                                break;

                            case 48:
                            case 16:
                                this->renderTable(false);
                                break;

                            case 32:
                                this->renderTable(true);
                                break;

                            case 0:
                                this->state.clearSelectedTableCards();
                                this->state.clearSelectedCard(currentPlayer);
                                this->renderTable(false);
                                this->renderLowerHand(currentPlayer, false, false);
                                this->state.setGameState(GameState::Play_SwapPlayers_Init);
                                break;

                        }

                        break;

                    case PLAYER_TWO:
                        switch (this->counter) {

                            case 96:
                            case 80:
                                this->renderTopHand(currentPlayer, true, false);
                                PD::update();
                                break;

                            case 88:
                            case 72:
                                this->renderTopHand(currentPlayer, true, true);
                                PD::update();
                                break;

                            case 64:
                                {
                                    Card selectedCard = this->state.getHandCard(currentPlayer, selectedIdx);
                                    this->state.removeHandCard(currentPlayer, selectedIdx, false);
                                    this->state.clearSelectedCard(currentPlayer);
                                    this->renderTopHand(currentPlayer, true, false);
                                    this->state.addTableCard(selectedCard);
                                    this->state.setSelectedTableCard(this->state.numberOfCardsOnTable() - 1, true);
                                    this->renderTable(true);
                                    PD::update();
                                }
                                break;

                            case 48:
                            case 16:
                                this->renderTable(false);
                                PD::update();
                                break;

                            case 32:
                                this->renderTable(true);
                                PD::update();
                                break;

                            case 0:
                                this->state.clearSelectedTableCards();
                                this->state.clearSelectedCard(currentPlayer);
                                this->renderTable(false);
                                this->renderTopHand(currentPlayer, true, true);
                                this->state.setGameState(GameState::Play_SwapPlayers_Init);
                                PD::update();
                                break;

                            default:
                                break;

                        }
                        
                        break;

                }

                break;

            case GameMode::PlayAndPass:

                switch (currentPlayer) {

                    case PLAYER_ONE:

                        switch (this->counter) {

                            case 96:
                            case 80:
                                this->renderLowerHand(currentPlayer, false);
                                break;

                            case 88:
                            case 72:
                                this->renderLowerHand(currentPlayer, true);
                                break;

                            case 64:
                                {
                                    Card selectedCard = this->state.getHandCard(currentPlayer, selectedIdx);
                                    this->state.removeHandCard(currentPlayer, selectedIdx, false);
                                    this->state.clearSelectedCard(currentPlayer);
                                    this->renderLowerHand(currentPlayer, false);
                                    this->state.addTableCard(selectedCard);
                                    this->state.setSelectedTableCard(this->state.numberOfCardsOnTable() - 1, true);
                                    this->renderTable(true);
                                }
                                break;

                            case 48:
                            case 16:
                                this->renderTable(false);
                                break;

                            case 32:
                                this->renderTable(true);
                                break;

                            case 0:
                                this->state.clearSelectedTableCards();
                                this->state.clearSelectedCard(currentPlayer);
                                this->renderTable(false);
                                this->renderLowerHand(currentPlayer, true, true);
                                this->state.setGameState(GameState::Play_SwapPlayers_Init);
                                break;

                        }

                        break;

                    case PLAYER_TWO:

                        switch (this->counter) {

                            case 96:
                            case 80:
                                this->renderLowerHand(currentPlayer, false);
                                break;

                            case 88:
                            case 72:
                                this->renderLowerHand(currentPlayer, true);
                                break;

                            case 64:
                                {
                                    Card selectedCard = this->state.getHandCard(currentPlayer, selectedIdx);
                                    this->state.removeHandCard(currentPlayer, selectedIdx, false);
                                    this->state.clearSelectedCard(currentPlayer);
                                    this->renderLowerHand(currentPlayer, false);
                                    this->state.addTableCard(selectedCard);
                                    this->state.setSelectedTableCard(this->state.numberOfCardsOnTable() - 1, true);
                                    this->renderTable(true);
                                }
                                break;

                            case 48:
                            case 16:
                                this->renderTable(false);
                                break;

                            case 32:
                                this->renderTable(true);
                                break;

                            case 0:
                                this->state.clearSelectedTableCards();
                                this->state.clearSelectedCard(currentPlayer);
                                this->renderTable(false);
                                this->renderLowerHand(currentPlayer, true, true);
                                this->state.setGameState(GameState::Play_SwapPlayers_Init);
                                break;

                        }

                        break;

                }

                break;

        }


    }

}


void Game::doPlay_SwapPlayers_Init() {

    uint8_t currentPlayer = this->state.getCurrentTurn();

    switch (this->state.getGameMode()) {

        case GameMode::SinglePlayer:

            switch (currentPlayer) {

                case PLAYER_ONE:

                    if (this->state.numberOfCardsInHand(PLAYER_ONE) == 0 && this->state.numberOfCardsInHand(PLAYER_TWO) == 0) {

                        if (this->state.numberOfCardsInDeck() > 0) { 

                            this->state.setGameState(GameState::Play_Deal_Init);

                        }
                        else {

                            if (this->state.numberOfCardsOnTable() > 0) {

                                this->state.setGameState(GameState::Play_LastCards_Init);

                            }
                            else {

                                this->doPlay_Score_Init();
                                this->state.setGameState(GameState::Play_Score);

                            }

                        }

                    }
                    else {

                        this->state.clearSelectedCard(this->state.getCurrentTurn());
                        this->state.swapPlayers();
                        this->state.setGameState(GameState::Play_Hand_Init);
                        PD::update();

                    }

                    return;

                case PLAYER_TWO:

                    this->state.clearSelectedCard(this->state.getCurrentTurn());
                    this->state.swapPlayers();
                    this->state.setSelectedCard(PLAYER_ONE, 0);

                    if (this->state.numberOfCardsInHand(PLAYER_ONE) == 0 && this->state.numberOfCardsInHand(PLAYER_TWO) == 0) {

                        if (this->state.numberOfCardsInDeck() > 0) { 

                            this->state.setGameState(GameState::Play_Deal_Init);

                        }
                        else {

                            if (this->state.numberOfCardsOnTable() > 0) {

                                this->state.setGameState(GameState::Play_LastCards_Init);

                            }
                            else {

                                this->doPlay_Score_Init();
                                this->state.setGameState(GameState::Play_Score);

                            }

                        }

                    }
                    else {

                        this->renderTable(false);
                        this->renderDeck();
                        this->renderPlayerName(PLAYER_ONE);
                        this->renderScore(PLAYER_ONE);
                        this->renderTopHand(PLAYER_TWO);
                        this->state.setGameState(GameState::Play_Hand_Init);
                        PD::update();

                    }

                    return;

            }

            break;

        case GameMode::PlayAndPass:

            if (this->state.numberOfCardsInHand(PLAYER_ONE) == 0 && this->state.numberOfCardsInHand(PLAYER_TWO) == 0) {

                if (this->state.numberOfCardsInDeck() > 0) { 

                    this->doPlay_SwapPlayers_RenderPanel(currentPlayer == PLAYER_ONE ? PLAYER_TWO : PLAYER_ONE, 0);
                    // this->renderPanel(PanelMessage::SwapPlayers);
                    this->state.setGameState(GameState::Play_SwapPlayers);                 

                }
                else {

                    if (this->state.numberOfCardsOnTable() > 0) {

                        this->state.setGameState(GameState::Play_LastCards_Init);

                    }
                    else {

                        this->doPlay_Score_Init();
                        this->state.setGameState(GameState::Play_Score);

                    }                    

                }

            }
            else {

                this->doPlay_SwapPlayers_RenderPanel(currentPlayer == PLAYER_ONE ? PLAYER_TWO : PLAYER_ONE, 0);
                // this->renderPanel(PanelMessage::SwapPlayers);
                this->state.setGameState(GameState::Play_SwapPlayers);
                PD::update();

            }
            
            break;

    }

}

void Game::doPlay_SwapPlayers() {

    if (PC::buttons.pressed(BTN_A)) {

        this->state.clearSelectedCard(this->state.getCurrentTurn());
        this->state.swapPlayers();

        uint8_t currentPlayer = this->state.getCurrentTurn();
        uint8_t alternatePlayer = (currentPlayer == PLAYER_ONE ? PLAYER_TWO : PLAYER_ONE);
        this->renderBackground(this->getBackgroundColourByTurn());

        switch (this->state.getGameMode()) {

            case GameMode::SinglePlayer:
                        
                this->state.setSelectedCard(currentPlayer, 0);
                this->renderTable(false);
                this->renderDeck();
                this->renderPlayerName(PLAYER_ONE);
                this->renderScore(PLAYER_ONE);
                this->renderTopHand(PLAYER_TWO);

                switch (currentPlayer) {
                    
                    case PLAYER_ONE:
                        this->renderLowerHand(PLAYER_ONE, true);
                        this->renderPointerHand(currentPlayer, PointerPosition::Hand);
                        break;

                    case PLAYER_TWO:
                        this->renderLowerHand(PLAYER_ONE, false);
                        break;

                }

                break;
        
            case GameMode::PlayAndPass:

                this->state.setSelectedCard(currentPlayer, 0);
                this->renderTable(false);
                this->renderDeck();
                this->renderPlayerName(currentPlayer);
                this->renderScore(currentPlayer);
                this->renderTopHand(alternatePlayer);
                this->renderLowerHand(currentPlayer, true);
                this->renderPointerHand(currentPlayer, PointerPosition::Hand);

                break;
                
        }

        if (this->state.numberOfCardsInHand(PLAYER_ONE) == 0 && this->state.numberOfCardsInHand(PLAYER_TWO) == 0) {

            if (this->state.numberOfCardsInDeck() > 0) { 
                this->state.setGameState(GameState::Play_Deal_Init);
            }
            else {
                this->state.setGameState(GameState::Play_Score_Init);
            }

        }
        else {

            this->state.setGameState(GameState::Play_Hand_Init);

        }

    }
    
}

void Game::doPlay_NominateDeckCards() {

    uint8_t currentPlayer = this->state.getCurrentTurn();
    uint8_t selectedIdx = this->state.getSelectedCard(currentPlayer);
    uint8_t numberOfCardsOnTable = this->state.numberOfCardsOnTable();

    Card selectedCard = this->state.getHandCard(currentPlayer, selectedIdx);

    switch (this->state.getGameState()) {

        case GameState::Play_NominateDeckCards_Single:

            if (PC::buttons.pressed(BTN_LEFT)) {

                bool found = false;

                for (uint8_t i = this->state.getPointer(currentPlayer, PointerPosition::Table); i > 0; i++) {

                    if (selectedCard.getNumber() == this->state.getTableCard(i - 1).getNumber()) {

                        this->state.setPointer(currentPlayer, PointerPosition::Table, i - 1);
                        found = true;
                        break;

                    }

                }

                if (found) {

                    this->renderTable(true);
                    this->renderPointerHand(currentPlayer, PointerPosition::Table);
                    PD::update();

                }

            }

            if (PC::buttons.pressed(BTN_RIGHT)) {

                bool found = false;

                for (uint8_t i = this->state.getPointer(currentPlayer, PointerPosition::Table) + 1; i < numberOfCardsOnTable; i++) {

                    if (selectedCard.getNumber() == this->state.getTableCard(i).getNumber()) {

                        this->state.setPointer(currentPlayer, PointerPosition::Table, i);
                        found = true;
                        break;

                    }
                }

                if (found) {

                    this->renderTable(true);
                    this->renderPointerHand(currentPlayer, PointerPosition::Table);
                    PD::update();

                }

            }

            if (PC::buttons.pressed(BTN_UP) || PC::buttons.pressed(BTN_A)) {

                this->state.setSelectedTableCard(this->state.getPointer(currentPlayer, PointerPosition::Table), true);
                this->state.setGameState(GameState::Play_TakeMatch);
                this->counter = 33;

            }
            break;

        case GameState::Play_NominateDeckCards_Multiple:

            if (PC::buttons.pressed(BTN_LEFT) && this->state.getPointer(currentPlayer, PointerPosition::Table) > 0) {

                this->state.setPointer(currentPlayer, PointerPosition::Table, this->state.getPointer(currentPlayer, PointerPosition::Table) - 1);
                this->renderTable(true);
                this->renderPointerHand(currentPlayer, PointerPosition::Table);
                PD::update();

            }

            if (PC::buttons.pressed(BTN_RIGHT) && this->state.getPointer(currentPlayer, PointerPosition::Table) < numberOfCardsOnTable - 1) {

                this->state.setPointer(currentPlayer, PointerPosition::Table, this->state.getPointer(currentPlayer, PointerPosition::Table) + 1);
                this->renderTable(true);
                this->renderPointerHand(currentPlayer, PointerPosition::Table);
                PD::update();

            }

            if (PC::buttons.pressed(BTN_UP)) {
                
                this->state.setSelectedTableCard(this->state.getPointer(currentPlayer, PointerPosition::Table), true);
                this->renderTable(true);
                this->renderPointerHand(currentPlayer, PointerPosition::Table);

            }

            if (PC::buttons.pressed(BTN_DOWN)) {
                
                this->state.setSelectedTableCard(this->state.getPointer(currentPlayer, PointerPosition::Table), false);
                this->renderTable(true);
                this->renderPointerHand(currentPlayer, PointerPosition::Table);

            }

            if (PC::buttons.pressed(BTN_A)) {

                uint8_t total = 0;

                while (true) {

                    for (uint8_t i = 0; i < numberOfCardsOnTable; i++) {

                        if (this->state.getSelectedTableCard(i)) {
                            total = total + this->state.getTableCard(i).getNumber() + 1;
                        }

                    }

                    if (selectedCard.getNumber() + 1 == total) {

                        this->state.setSelectedTableCard(this->state.getPointer(currentPlayer, PointerPosition::Table), true);
                        this->state.setGameState(GameState::Play_TakeMatch);
                        this->counter = 33;

                    }
                    else {

                        if (!this->state.getSelectedTableCard(this->state.getPointer(currentPlayer, PointerPosition::Table))) {

                            this->state.setSelectedTableCard(this->state.getPointer(currentPlayer, PointerPosition::Table), true);
                            this->renderTable(true);

                        }
                        else {
                            break;
                        }

                    }

                }

            }

            break;

    }

}

void Game::doPlay_Scopa_Init() {

    //PD::drawBitmap(40, 50, Images::Scopa);
    PD::drawBitmap(51, 59, Images::Scopa);
    this->state.setGameState(GameState::Play_Scopa);
    this->playSoundEffect(SoundTheme::Scopa);
    PD::update();

}

void Game::doPlay_Scopa() {

    if (PC::buttons.pressed(BTN_A)) {
        this->state.setGameState(GameState::Play_SwapPlayers_Init);
    }

}

void Game::debugHands() {

    #ifdef DEBUG
    printf("-------------------------------------------------------\n");
    printf("Player 1: ");
    for (uint8_t i = 0; i < this->state.numberOfCardsInHand(0); i++) {
        printf("%i ", this->state.getHandCard(0, i).getCardIdx());
    }
    printf(", Player 2: ");
    for (uint8_t i = 0; i < this->state.numberOfCardsInHand(1); i++) {
        printf("%i ", this->state.getHandCard(1, i).getCardIdx());
    }
    printf(", Table: ");
    for (uint8_t i = 0; i < this->state.numberOfCardsOnTable(); i++) {
        printf("%i ", this->state.getTableCard(i).getCardIdx());
    }
    printf(", DeckIndex %i, In Deck %i\n", this->state.getDeckIndex(), this->state.numberOfCardsInDeck());
    #endif

}


void Game::doPlay_Hand_ComputerAction() {

    
    uint8_t matchesArray[20] =      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };

    
    // Sette Bello?

    uint8_t playerSetteBelloIdx = NO_CARD;
    uint8_t playerOtherSevenIdx = NO_CARD;
    uint8_t tableSetteBelloIdx = NO_CARD;
    uint8_t tableOtherSevenIdx = NO_CARD;

    playerSetteBelloIdx = this->state.playerHasExactCard(PLAYER_TWO, SUIT_COINS + CARD_SEVEN);
    if (!playerSetteBelloIdx) playerOtherSevenIdx = this->state.playerHasNumberCard(PLAYER_TWO, CARD_SEVEN);

    tableSetteBelloIdx = this->state.tableHasExactCard(SUIT_COINS + CARD_SEVEN);
    if (!tableSetteBelloIdx) tableOtherSevenIdx = this->state.tableHasNumberCard(CARD_SEVEN);

    if (playerOtherSevenIdx != NO_CARD && tableSetteBelloIdx != NO_CARD) {

        this->state.setSelectedCard(PLAYER_TWO, playerOtherSevenIdx);
        this->state.setSelectedTableCard(PLAYER_TWO, tableSetteBelloIdx);
        this->state.setGameState(GameState::Play_TakeMatch);
        this->counter = 97;

        #ifdef DEBUG
        printf("Play: Table 7 Coins\n");        
        #endif

        return;
        
    }

    if (playerSetteBelloIdx != NO_CARD) {

        if (tableOtherSevenIdx != NO_CARD) {

            this->state.setSelectedCard(PLAYER_TWO, playerSetteBelloIdx);
            this->state.setSelectedTableCard(PLAYER_TWO, tableOtherSevenIdx);
            this->state.setGameState(GameState::Play_TakeMatch);
            this->counter = 193; 

            #ifdef DEBUG
            printf("Play: Player 7 Coins\n");        
            #endif

            return;

        }

        // Can we match anything else?

        bool match = this->state.tableHasMatch(this->state.getHandCard(PLAYER_TWO, playerSetteBelloIdx), matchesArray, check);

        if (match > 0) {

            #ifdef DEBUG
            printf("Play: Match of 7 coins against ");        
            #endif

            this->state.setSelectedCard(PLAYER_TWO, playerSetteBelloIdx);

            for (uint8_t i = 0; i < 20; i++) {

                if (matchesArray[i] != NO_CARD) {

                    #ifdef DEBUG
                    printf("%i (%i) ", matchesArray[i], this->state.getTableCard(matchesArray[i]).getCardIdx());
                    #endif

                    this->state.setSelectedTableCard(matchesArray[i], true);
                }

            }

            #ifdef DEBUG
            printf("\n");
            #endif

            this->state.setGameState(GameState::Play_TakeMatch);
            this->counter = 193;
            return;

        }
        
    }


    // Can we get a scopa?

    if (this->state.numberOfCardsInHand(PLAYER_TWO) == 1 && this->state.getHandCard(PLAYER_TWO, 0).getNumber() + 1 == this->state.tableValue()) {

        #ifdef DEBUG
        printf("Play: Scopa!\n ");   
        #endif

        this->state.setSelectedCard(PLAYER_TWO, 0);

        for (uint8_t i = 0; i < this->state.numberOfCardsOnTable(); i++) {

            this->state.setSelectedTableCard(i, true);

        }

        this->state.setGameState(GameState::Play_TakeMatch);
        this->counter = 193;
        return;   

    }


    // Is there a match?
    
    uint8_t playerCard = 0;

    if (this->state.tableBestPlay(PLAYER_TWO, playerCard, matchesArray, check)) {

        #ifdef DEBUG
        printf("Play: Match of %i (%i) against ", playerCard, this->state.getHandCard(PLAYER_TWO, playerCard).getCardIdx());    
        #endif

        this->state.setSelectedCard(PLAYER_TWO, playerCard);

        for (uint8_t i = 0; i < 20; i++) {

            if (matchesArray[i] != NO_CARD) {

                #ifdef DEBUG
                printf("%i (%i) ", matchesArray[i], this->state.getTableCard(matchesArray[i]).getCardIdx());
                #endif

                this->state.setSelectedTableCard(matchesArray[i], true);
            }

        }

        #ifdef DEBUG
        printf("\n");
        #endif

        this->state.setGameState(GameState::Play_TakeMatch);
        this->counter = 193;
        return;

    }



    
    // If there are no matches then add a card to the table ..

    playerCard = this->state.bestDiscard(PLAYER_TWO);

    #ifdef DEBUG
    printf("Play: Best discard %i (%i)\n ", playerCard, this->state.getHandCard(PLAYER_TWO, playerCard).getCardIdx());      
    #endif

    this->state.setSelectedCard(PLAYER_TWO, playerCard);
    this->state.setGameState(GameState::Play_AddCardToHand);
    this->counter = 97;
    
}
