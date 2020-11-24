
#include "Pokitto.h"
#include "Game.h"
#include "utils/Consts.h"
#include "images/Images.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;

void Game::renderBackground(uint8_t colour, int16_t x, int16_t y, uint16_t w, uint16_t h) {

    #if COLOUR_P2 == 255

    if (x == 255) {

        switch (colour) {

            case COLOUR_P1:
                this->fillRectAlt(0, 0, PD::getWidth(), PD::getHeight(), COLOUR_P1, COLOUR_P1_ALT);
                break;

            case COLOUR_P2:
                this->fillRectAlt(0, 0, PD::getWidth(), PD::getHeight(), COLOUR_P1, COLOUR_P2_ALT);
                break;


        }

    }
    else {

        switch (colour) {

            case COLOUR_P1:
                this->fillRectAlt(x, y, w, h, COLOUR_P1, COLOUR_P1_ALT);
                break;

            case COLOUR_P2:
                this->fillRectAlt(x, y, w, h, COLOUR_P1, COLOUR_P2_ALT);
                break;


        }

    }

    #else

        if (x == 255) {

            switch (colour) {

                case COLOUR_P1:
                    this->fillRectAlt(0, 0, PD::getWidth(), PD::getHeight(), COLOUR_P1, COLOUR_P1_ALT);
                    break;

                case COLOUR_P2:
                    this->fillRectAlt(0, 0, PD::getWidth(), PD::getHeight(), COLOUR_P2, COLOUR_P2_ALT);
                    break;


            }

        }
        else {

            switch (colour) {

                case COLOUR_P1:
                    this->fillRectAlt(x, y, w, h, COLOUR_P1, COLOUR_P1_ALT);
                    break;

                case COLOUR_P2:
                    this->fillRectAlt(x, y, w, h, COLOUR_P2, COLOUR_P2_ALT);
                    break;


            }

        }

    #endif
}

void Game::renderLargeCard(uint8_t x, int16_t y, Card card, uint8_t rotation, bool topCorner) {

    this->readImage(imgBuffer, card, rotation, (y <= 80 ? 98 : 98 - (y - 80)));
    if (topCorner) PD::drawBitmap(x, y, Images::Card_Large_TopCorner);
    PD::drawBitmap(x, y, imgBuffer);

}

void Game::renderLargeCard_Top(uint8_t x, int16_t y, Card card, uint8_t rotation, bool topCorner) {

    PD::drawBitmap(x, y + 70, Images::Card_Large_Face_00);
    PD::drawBitmap(x + 58 - (card.getNumber() < 9 ? 0 : 8), y + 86, Images::Cards_Small_Numbers[card.getNumber()]);
    PD::drawBitmap(x + 24, y + 66, Images::Cards_Small_Suits[card.getSuit()]);

    if (card.getNumber() == 5 || card.getNumber() == 8) {

        PD::drawBitmap(x + 58, y + 94, Images::Card_Small_Underline);

    }

}


void Game::renderSmallCard(uint8_t x, uint8_t y, Card card, bool highlight) {

    if (highlight) {
        PD::drawBitmap(x, y, Images::Card_Small_Front_Selected);
    }
    else {
        PD::drawBitmap(x, y, Images::Card_Small_Front);
    }

    PD::drawBitmap(x + 4, y + 4, Images::Cards_Small_Numbers[card.getNumber()]);
    PD::drawBitmap(x + 9, y + 15, Images::Cards_Small_Suits[card.getSuit()]);

}

void Game::renderLowerHand_Helper(uint8_t player, uint8_t cardNo, int16_t x, int16_t y, bool highlight, bool hide, bool topCorner) {

    if (hide) {
        PD::drawBitmap(x, y, Images::Cards_Large_Back[0]);
    }
    else {
        if (this->state.getPointer(player, PointerPosition::Hand) != cardNo) {
            this->renderLargeCard(x, y, this->state.getHandCard(player, cardNo), ROTATION_NONE);
        }
        else {
            this->renderLargeCard(x, y - 10, this->state.getHandCard(player, cardNo), ROTATION_NONE, topCorner);
            if (highlight) PD::drawBitmap(x, PLAYER_TWO_CARD_Y - 10, Images::Card_Large_Highlight);
        }
    }

}

void Game::renderLowerHand(uint8_t player, bool highlight, bool hide) {

    PD::setColor(this->getBackgroundColourByTurn(), this->getBackgroundColourByTurn());
    this->renderBackground(this->getBackgroundColourByTurn(), PLAYER_ONE_CARD_1_X, PLAYER_ONE_CARD_Y - 10, (PLAYER_CARD_SPACING * 2) + PLAYER_CARD_WIDTH, PLAYER_CARD_HEIGHT);

    switch (this->state.numberOfCardsInHand(player)) {

        case 0:
            break;

        case 1:
            this->renderLowerHand_Helper(player, 0, PLAYER_ONE_CARD_2_X, PLAYER_ONE_CARD_Y, highlight, hide, false);
            break;

        case 2:
            this->renderLowerHand_Helper(player, 0, PLAYER_ONE_CARD_1H_X, PLAYER_ONE_CARD_Y, highlight, hide, false);
            this->renderLowerHand_Helper(player, 1, PLAYER_ONE_CARD_2H_X, PLAYER_ONE_CARD_Y, highlight, hide, this->state.getPointer(player, PointerPosition::Hand) != 1);
            break;

        case 3:
            this->renderLowerHand_Helper(player, 0, PLAYER_ONE_CARD_1_X, PLAYER_ONE_CARD_Y, highlight, hide, false);
            this->renderLowerHand_Helper(player, 1, PLAYER_ONE_CARD_2_X, PLAYER_ONE_CARD_Y, highlight, hide, this->state.getPointer(player, PointerPosition::Hand) != 1);
            this->renderLowerHand_Helper(player, 2, PLAYER_ONE_CARD_3_X, PLAYER_ONE_CARD_Y, highlight, hide, this->state.getPointer(player, PointerPosition::Hand) != 2);
            break;

    }
  
}

void Game::renderTopHand_Helper(uint8_t player, uint8_t cardNo, uint8_t selectedCard, int16_t x, bool showCard, bool highlight, bool omitLowerCorner) {

    if (showCard && selectedCard == cardNo) {
        Card card = this->state.getHandCard(player, cardNo);
        this->renderLargeCard_Top(x, -70, card, 0, highlight);
    }
    else {
        PD::drawBitmap(x, 0, omitLowerCorner ? Images::Card_Large_Bottom_Left : Images::Card_Large_Bottom_Others);
    }

}

void Game::renderTopHand(uint8_t player, bool showCard, bool highlight) {

    uint8_t selectedCard = this->state.getSelectedCard(player);
    this->renderBackground(this->getBackgroundColourByTurn(), PLAYER_TWO_CARD_1_X, 0, (PLAYER_CARD_SPACING * 2) + PLAYER_CARD_WIDTH, 28);

    switch (this->state.numberOfCardsInHand(player)) {

        case 0:
            break;

        case 1:
            this->renderTopHand_Helper(player, 0, selectedCard, PLAYER_TWO_CARD_2_X, showCard, highlight, true);
            break;

        case 2:
            this->renderTopHand_Helper(player, 0, selectedCard, PLAYER_TWO_CARD_1H_X, showCard, highlight, true);
            this->renderTopHand_Helper(player, 1, selectedCard, PLAYER_TWO_CARD_2H_X, showCard, highlight, showCard && selectedCard == 0);
            break;

        case 3:
            this->renderTopHand_Helper(player, 0, selectedCard, PLAYER_TWO_CARD_1_X, showCard, highlight, true);
            this->renderTopHand_Helper(player, 1, selectedCard, PLAYER_TWO_CARD_2_X, showCard, highlight, showCard && selectedCard == 0);
            this->renderTopHand_Helper(player, 2, selectedCard, PLAYER_TWO_CARD_3_X, showCard, highlight, showCard && selectedCard == 1);
            break;

    }
  
}

void Game::renderTable(bool highlight) {

    uint8_t numberOfCards = this->state.numberOfCardsOnTable();
    uint8_t leftSide = 125 - (((numberOfCards * 20) + 20) / 2);

    this->renderBackground(this->getBackgroundColourByTurn(), 28, 32, 220, 65);

    for (uint8_t i = 0; i < numberOfCards; i++) {

        if (this->state.getSelectedTableCard(i)) {

            this->renderSmallCard(leftSide + (i * 20), (numberOfCards > 1 ? 32 : 39), this->state.getTableCard(i), highlight);

        }
        else {
            this->renderSmallCard(leftSide + (i * 20), 39, this->state.getTableCard(i));
        }

    }
    
}


void Game::renderPointerHand(uint8_t player, PointerPosition position) {

    switch (position) {

        case PointerPosition::Hand:
            {

                uint8_t cardIdx = this->state.getPointer(player, PointerPosition::Hand);

                switch (this->state.numberOfCardsInHand(player)) {

                    case 0:
                        break;

                    case 1:
                        PD::drawBitmap(PLAYER_ONE_CARD_2_X + 8, PLAYER_ONE_CARD_Y + 35, Images::Hand_Large);
                        break;

                    case 2:
                        PD::drawBitmap(PLAYER_ONE_CARD_1H_X + 8 + (cardIdx * PLAYER_CARD_SPACING), PLAYER_ONE_CARD_Y + 35, Images::Hand_Large);
                        break;

                    case 3:
                        PD::drawBitmap(PLAYER_ONE_CARD_1_X + 8 + (cardIdx * PLAYER_CARD_SPACING), PLAYER_ONE_CARD_Y + 35, Images::Hand_Large);
                        break;

                }

            }

            break;

        case PointerPosition::Table:
            {
                uint8_t cardIdx = this->state.getPointer(player, PointerPosition::Table);
                uint8_t numberOfCards = this->state.numberOfCardsOnTable();
                uint8_t leftSide = 130 - (((numberOfCards * 20) + 20) / 2);
                
                PD::drawBitmap(leftSide + (cardIdx * 20), 56, Images::Hand);         
            }
            break;

    }

}

void Game::renderDeck() {

    uint8_t numberOfCardsInDeck = this->state.numberOfCardsInDeck();

    if (numberOfCardsInDeck > 0) numberOfCardsInDeck--;

    PD::drawBitmap(-20, 37, Images::Card_Small_Deck[numberOfCardsInDeck / 5]);

    uint8_t deckCount_Maj_1 = this->state.numberOfCardsInDeck() / 10;
    uint8_t deckCount_Min_1 = this->state.numberOfCardsInDeck() % 10;

    PD::setColor(this->getBackgroundColourByTurn(), this->getBackgroundColourByTurn());
    PD::fillRect(3, 100, 17, 8);

    PD::setCursor(4, 101);
    PD::setColor(6, 14);

    if (deckCount_Maj_1 == 0) {
        PD::print("0");
    }
    else {
        PD::print(deckCount_Maj_1, 10);
    }
    PD::setCursor(11, 101);
    PD::print(deckCount_Min_1, 10);


}


void Game::readImage(uint8_t *buffer, Card card, uint8_t rotation, uint8_t height) {

    const uint32_t widths[10] = { 70, 64, 56, 50, 42, 36, 28, 22, 14, 8 };
    const uint32_t widths_Cum[10] = { 0, 70, 134, 190, 240, 282, 318, 346, 368, 382 };

    const uint32_t startPos = (SDCardLength * card.getCardIdx()) + (rotation * 2) + (widths_Cum[rotation] * 98 / 2);

    file.seek(startPos);
    file.read(buffer, 2 + (widths[rotation] * height / 2));
    buffer[1] = height;

}

void Game::fillRectAlt(int x0,int y0, int w, int h, uint8_t colour1, uint8_t colour2) {
    
    int16_t x,y,x1,y1;
    
    x1=x0+w;
    y1=y0+h;

    if ((x0<0 && x1<0) || (x0>=PD::getWidth() && x1 >=PD::getWidth()))    return; //completely out of bounds
    if ((y0<0 && y1<0) || (y0>=PD::getHeight() && y1 >=PD::getHeight()))  return; //completely out of bounds
    
    if (x0>x1) {
        x = x1;
        x1 = x0;
    }
    else x = x0;

    if (y0>y1) {
        y = y1;
        y1 = y0;
    }
    else y = y0;

    if (x<0) x=0;
    if (y<0) y=0;

    for (int16_t i = y0; i < y1; i++ ) {

        if (i >= 30 && i < 145) {
            PD::setColor(i % 2 == 1 ? colour1 : colour2);
        }
        else {
            PD::setColor(colour1);
        }
        PD::drawRow(x0,x1,i);
    }

}

void Game::renderPlayerName(uint8_t botPlayer) {

    switch (botPlayer) {

        case PLAYER_ONE:

            PD::setColor(6, COLOUR_P1);

            switch (this->state.getGameMode()){

                case GameMode::SinglePlayer:
                    // this->renderString("COMPUTER", 9, 154, 3);
                    break;

                case GameMode::PlayAndPass:
                    this->renderString(this->cookie->initials[0], this->cookie->getPlayerLength(PLAYER_ONE), 2, 165);
                    this->renderString(this->cookie->initials[1], this->cookie->getPlayerLength(PLAYER_TWO), 218 - (this->cookie->getPlayerLength(PLAYER_TWO) * 8), 3);
                    break;

            }

            break;

        case PLAYER_TWO:

            switch (this->state.getGameMode()){

                case GameMode::SinglePlayer:
                    break;

                case GameMode::PlayAndPass:
                    this->renderString(this->cookie->initials[1], this->cookie->getPlayerLength(PLAYER_TWO), 2, 165);
                    this->renderString(this->cookie->initials[0], this->cookie->getPlayerLength(PLAYER_ONE), 218 - (this->cookie->getPlayerLength(PLAYER_ONE) * 8), 3);
                    break;

            }

    }

}

void Game::renderScore_Individual(uint8_t x, uint8_t y, uint8_t score) {

    uint8_t score_Maj = score / 10;
    uint8_t score_Min = score % 10;

    PD::drawBitmap(x, y, Images::Score);

    PD::setCursor(x + 2, y + 3);
    if (score_Maj == 0) {
        PD::print("0");
    }
    else {
        PD::print(score_Maj, 10);
    }

    PD::setCursor(x + 9, y + 3);
    PD::print(score_Min, 10);

}

void Game::renderScore(uint8_t botPlayer) {

    PD::setColor(6, 0);

    switch (botPlayer) {

        case PLAYER_ONE:

            switch (this->state.getGameMode()) {
                
                case GameMode::SinglePlayer:
                    this->renderScore_Individual(2, 160, this->state.getScore(PLAYER_ONE));
                    this->renderScore_Individual(199, 1, this->state.getScore(PLAYER_TWO));
                    break;

                case GameMode::PlayAndPass:
                    this->renderScore_Individual(2, 147, this->state.getScore(PLAYER_ONE));
                    this->renderScore_Individual(199, 13, this->state.getScore(PLAYER_TWO));
                    break;

            }

            break;

        case PLAYER_TWO:

            switch (this->state.getGameMode()) {
                
                case GameMode::SinglePlayer:
                    this->renderScore_Individual(2, 160, this->state.getScore(PLAYER_TWO));
                    this->renderScore_Individual(199, 1, this->state.getScore(PLAYER_ONE));
                    break;

                case GameMode::PlayAndPass:
                    this->renderScore_Individual(2, 147, this->state.getScore(PLAYER_TWO));
                    this->renderScore_Individual(199, 13, this->state.getScore(PLAYER_ONE));
                    break;

            }

            break;

    }

    switch (this->state.getGameMode()) {
        
        case GameMode::SinglePlayer:
            break;

        case GameMode::PlayAndPass:
            PD::drawBitmap(25, 147, Images::TalkBubble);
            break;

    }

}


const uint8_t messagesTextOffset[] = {
  26,
  4, 
  4,
  16, 
  4,
  24,
  24,
  36,
  24,
  37,
  20,
  20,
  8,
  12
};

char * messagesText[] = {
  "SWAP PLAYERS",
  "YOUR PICK PLAYER 1",
  "YOUR PICK PLAYER 2",
  "YOUR PICK HUMAN",
  "YOUR PICK COMPUTER",
  "PLAYER 1 WINS",
  "PLAYER 2 WINS",
  "HUMAN WINS",
  "COMPUTER WINS",
  "YOU DREW !",
  "P1 TAKES CARDS",
  "P2 TAKES CARDS",
  "HUMAN TAKES CARDS",
  "COMP TAKES CARDS",
};


void Game::renderPanel(PanelMessage panelMessage, int8_t yOffset) {

    uint8_t messageNumber = static_cast<uint8_t>(panelMessage);

    PD::drawBitmap(30, 30 + yOffset, Images::Panel);
    this->renderPanelMessage(messagesText[messageNumber], messagesTextOffset[messageNumber], yOffset);

}

void Game::renderPanelMessage(char message[], int8_t xOffset, int8_t yOffset) {

    PD::setColor(4, 14);
    PD::setCursor(33 + xOffset, 77 + yOffset);
    PD::print(message);
    PD::setCursor(33 + xOffset, 76 + yOffset);
    PD::print(message);
    PD::setColor(9, 14);
    PD::setCursor(33 + xOffset, 75 + yOffset);
    PD::print(message);

}

void Game::renderString(char message[], uint8_t messageLen, uint8_t x, uint8_t y) {

    for (uint8_t i = 0; i < messageLen; i++) {

        switch (message[i]) {

            case ASCII_START ... ASCII_END:
            case ASCII_PERIOD:
                PD::setCursor(x + (i * 8), y);
                PD::print(message[i]);
                break;

            case KEY_SMILEY_FACE:
                PD::drawBitmap(x + (i * 8), y, Images::FaceSmiley);
                break;

            case KEY_FROWN_FACE:
                PD::drawBitmap(x + (i * 8), y, Images::FaceFrown);
                break;

        }

    }

}

void Game::doPlay_SwapPlayers_RenderPanel(uint8_t player, int8_t yOffset) {

    uint8_t len = this->cookie->getPlayerLength(player);
    PD::drawBitmap(30, 30 + yOffset, Images::Panel);

    char name[] = { 'Y', 'O', 'U', 'R', ' ', 'T', 'U', 'R', 'N', ' ', '1', '2', '3', '4', '5', '6', '7', '8', 0 };
    for (uint8_t i = 0; i < 8; i++) {
        name[10 + i] = this->cookie->getPlayerChar(player, i);
    }

    this->renderPanelMessage(name, (8 - (len - 1)) * 8 / 2, yOffset);

}