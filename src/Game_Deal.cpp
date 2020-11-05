
#include "Pokitto.h"
#include "Game.h"
#include "utils/Consts.h"
#include "images/Images.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;

void Game::doPlay_Deal_Init() {

    this->dealSequenceVariables.playSeq = 0;
    this->dealSequenceVariables.x = 0;
    this->dealSequenceVariables.y = 0;
    this->dealSequenceVariables.xOld = 0;
    this->dealSequenceVariables.yOld = 0;
    this->dealSequenceVariables.rotOld = 0;
    this->dealSequenceVariables.startOfDeck = this->state.getDeckIndex();
    this->dealSequenceVariables.numberOfCardsOnTable = this->state.numberOfCardsOnTable();

    this->state.setGameState(GameState::Play_Deal);


    // Clear deck from the image ..

    this->state.setCurrentDeal(this->state.getCurrentDeal());
    uint8_t currentDealer = this->state.getCurrentDeal();
    uint8_t alternate = (currentDealer = PLAYER_ONE ? PLAYER_TWO : PLAYER_ONE);

    switch (currentDealer) {

        case PLAYER_ONE:
            this->renderBackground(this->getBackgroundColourByDeal(), 0, 37, 24, 78);
            this->renderBackground(this->getBackgroundColourByDeal(), 2, 147, 80, 27);
            this->renderBackground(this->getBackgroundColourByDeal(), 148, 1, 80, 27);
            break;

        case PLAYER_TWO:

            switch (this->state.getGameMode()) {

                case GameMode::SinglePlayer:
                    this->renderBackground(this->getBackgroundColourByDeal(), 0, 37, 24, 78);
                    this->renderBackground(this->getBackgroundColourByDeal(), 2, 147, 80, 25);
                    this->renderBackground(this->getBackgroundColourByDeal(), 148, 3, 80, 25);
                    break;

                case GameMode::PlayAndPass:
                    this->renderBackground(this->getBackgroundColourByDeal());
                    this->renderLowerHand(currentDealer, false);
                    this->renderTopHand(alternate);
                    this->renderTable(false);
                    this->renderDeck();
                    this->renderPlayerName(currentDealer);
                    this->renderScore(currentDealer);
                    break;

            }

            break;
        
    }


}

void Game::doPlay_Deal() {

    const uint32_t widths[11] = { 70, 64, 56, 50, 42, 36, 28, 22, 14, 8, 4 };
    uint8_t currentDealer = this->state.getCurrentDeal();


    // Render player name..
    switch (currentDealer) {

        case PLAYER_ONE:

            switch (this->dealSequenceVariables.playSeq) {

                case DEAL_BOT_PLAYER_1_CARD_1_START:
                    this->dealSequenceVariables.x = 220;
                    this->dealSequenceVariables.y = 70;
                    this->dealSequenceVariables.card = this->state.drawCard();
                    this->doPlay_Deal_Action(HandPosition::Bottom, 0, this->dealSequenceVariables.playSeq - DEAL_BOT_PLAYER_1_CARD_1_START, this->dealSequenceVariables.card);
                    this->playSoundEffect(SoundTheme::Deal_01);
                    break;
                    
                case DEAL_BOT_PLAYER_1_CARD_1_START + 1 ... DEAL_BOT_PLAYER_1_CARD_1_END - 1:
                    this->renderBackground(COLOUR_P1, this->dealSequenceVariables.xOld, this->dealSequenceVariables.yOld, widths[this->dealSequenceVariables.rotOld], 98);
                    this->renderTable(false);
                    this->doPlay_Deal_Action(HandPosition::Bottom, 0, this->dealSequenceVariables.playSeq - DEAL_BOT_PLAYER_1_CARD_1_START, this->dealSequenceVariables.card);
                    break;
                    
                case DEAL_BOT_PLAYER_1_CARD_1_END:
                    this->renderBackground(COLOUR_P1, this->dealSequenceVariables.xOld, this->dealSequenceVariables.yOld, widths[this->dealSequenceVariables.rotOld], 98);
                    this->renderTable(false);
                    this->state.setHandCard(PLAYER_ONE, 0, this->dealSequenceVariables.card);
                    this->renderLargeCard(PLAYER_ONE_CARD_1_X, PLAYER_ONE_CARD_Y, this->dealSequenceVariables.card, ROTATION_NONE);
                    break;

                case DEAL_BOT_PLAYER_2_CARD_1_START:
                    this->dealSequenceVariables.x = 218;
                    this->dealSequenceVariables.y = 10;
                    this->dealSequenceVariables.card = this->state.drawCard();
                    this->doPlay_Deal_Action(HandPosition::Top, 0, this->dealSequenceVariables.playSeq - DEAL_BOT_PLAYER_2_CARD_1_START, this->dealSequenceVariables.card);
                    this->playSoundEffect(SoundTheme::Deal_02);
                    break;
                    
                case DEAL_BOT_PLAYER_2_CARD_1_START + 1 ... DEAL_BOT_PLAYER_2_CARD_1_END - 1:
                    this->renderBackground(COLOUR_P1, this->dealSequenceVariables.xOld, this->dealSequenceVariables.yOld, widths[this->dealSequenceVariables.rotOld], 98);
                    this->renderTable(false);
                    this->doPlay_Deal_Action(HandPosition::Top, 0, this->dealSequenceVariables.playSeq - DEAL_BOT_PLAYER_2_CARD_1_START, this->dealSequenceVariables.card);
                    break;
                    
                case DEAL_BOT_PLAYER_2_CARD_1_END:
                    this->state.setHandCard(PLAYER_TWO, 0, this->dealSequenceVariables.card);
                    break;

                case DEAL_BOT_PLAYER_1_CARD_2_START:
                    this->dealSequenceVariables.x = 220;
                    this->dealSequenceVariables.y = 70;
                    this->dealSequenceVariables.card = this->state.drawCard();
                    this->doPlay_Deal_Action(HandPosition::Bottom, 1, this->dealSequenceVariables.playSeq - DEAL_BOT_PLAYER_1_CARD_2_START, this->dealSequenceVariables.card);
                    this->playSoundEffect(SoundTheme::Deal_01);
                    break;
                    
                case DEAL_BOT_PLAYER_1_CARD_2_START + 1 ... DEAL_BOT_PLAYER_1_CARD_2_END - 1:
                    this->renderBackground(COLOUR_P1, this->dealSequenceVariables.xOld, this->dealSequenceVariables.yOld, widths[this->dealSequenceVariables.rotOld], 98);
                    this->renderTable(false);
                    this->doPlay_Deal_Action(HandPosition::Bottom, 1, this->dealSequenceVariables.playSeq - DEAL_BOT_PLAYER_1_CARD_2_START, this->dealSequenceVariables.card);
                    break;
                    
                case DEAL_BOT_PLAYER_1_CARD_2_END:  
                    this->renderBackground(COLOUR_P1, this->dealSequenceVariables.xOld, this->dealSequenceVariables.yOld, widths[this->dealSequenceVariables.rotOld], 98);
                    this->renderTable(false);
                    this->state.setHandCard(PLAYER_ONE, 1, this->dealSequenceVariables.card);
                    this->renderLargeCard(PLAYER_ONE_CARD_2_X, PLAYER_ONE_CARD_Y, this->dealSequenceVariables.card, ROTATION_NONE, true);
                    break;

                case DEAL_BOT_PLAYER_2_CARD_2_START:
                    this->dealSequenceVariables.x = 218;
                    this->dealSequenceVariables.y = 10;
                    this->dealSequenceVariables.card = this->state.drawCard();
                    this->doPlay_Deal_Action(HandPosition::Top, 1, this->dealSequenceVariables.playSeq - DEAL_BOT_PLAYER_2_CARD_2_START, this->dealSequenceVariables.card);
                    this->playSoundEffect(SoundTheme::Deal_02);
                    break;
                    
                case DEAL_BOT_PLAYER_2_CARD_2_START + 1 ... DEAL_BOT_PLAYER_2_CARD_2_END - 1:
                    this->renderBackground(COLOUR_P1, this->dealSequenceVariables.xOld, this->dealSequenceVariables.yOld, widths[this->dealSequenceVariables.rotOld], 98);
                    this->renderTable(false);
                    this->doPlay_Deal_Action(HandPosition::Top, 1, this->dealSequenceVariables.playSeq - DEAL_BOT_PLAYER_2_CARD_2_START, this->dealSequenceVariables.card);
                    break;
                    
                case DEAL_BOT_PLAYER_2_CARD_2_END:
                    PD::drawBitmap(PLAYER_TWO_CARD_2_X, 0, Images::Card_Large_Bottom_Others);
                    this->state.setHandCard(PLAYER_TWO, 1, this->dealSequenceVariables.card);
                    break;

                case DEAL_BOT_PLAYER_1_CARD_3_START:
                    this->dealSequenceVariables.x = 220;
                    this->dealSequenceVariables.y = 70;
                    this->dealSequenceVariables.card = this->state.drawCard();
                    this->doPlay_Deal_Action(HandPosition::Bottom, 2, this->dealSequenceVariables.playSeq - DEAL_BOT_PLAYER_1_CARD_3_START, this->dealSequenceVariables.card);
                    this->playSoundEffect(SoundTheme::Deal_01);
                    break;
                    
                case DEAL_BOT_PLAYER_1_CARD_3_START + 1 ... DEAL_BOT_PLAYER_1_CARD_3_END - 1:
                    this->renderBackground(COLOUR_P1, this->dealSequenceVariables.xOld, this->dealSequenceVariables.yOld, widths[this->dealSequenceVariables.rotOld], 98);
                    this->renderTable(false);
                    this->doPlay_Deal_Action(HandPosition::Bottom, 2, this->dealSequenceVariables.playSeq - DEAL_BOT_PLAYER_1_CARD_3_START, this->dealSequenceVariables.card);
                    break;
                    
                case DEAL_BOT_PLAYER_1_CARD_3_END:
                    this->renderBackground(COLOUR_P1, this->dealSequenceVariables.xOld, this->dealSequenceVariables.yOld, widths[this->dealSequenceVariables.rotOld], 98);
                    this->renderTable(false);
                    this->state.setHandCard(PLAYER_ONE, 2, this->dealSequenceVariables.card);
                    this->renderLargeCard(PLAYER_ONE_CARD_3_X, PLAYER_ONE_CARD_Y, this->dealSequenceVariables.card, ROTATION_NONE, true);
                    break;            

                case DEAL_BOT_PLAYER_2_CARD_3_START:
                    this->dealSequenceVariables.x = 218;
                    this->dealSequenceVariables.y = 10;
                    this->dealSequenceVariables.card = this->state.drawCard();
                    this->doPlay_Deal_Action(HandPosition::Top, 2, this->dealSequenceVariables.playSeq - DEAL_BOT_PLAYER_2_CARD_3_START, this->dealSequenceVariables.card);
                    this->playSoundEffect(SoundTheme::Deal_01);
                    break;
                    
                case DEAL_BOT_PLAYER_2_CARD_3_START + 1 ... DEAL_BOT_PLAYER_2_CARD_3_END - 1:
                    this->renderBackground(COLOUR_P1, this->dealSequenceVariables.xOld, this->dealSequenceVariables.yOld, widths[this->dealSequenceVariables.rotOld], 98);
                    this->renderTable(false);
                    this->doPlay_Deal_Action(HandPosition::Top, 2, this->dealSequenceVariables.playSeq - DEAL_BOT_PLAYER_2_CARD_3_START, this->dealSequenceVariables.card);
                    break;
                    
                case DEAL_BOT_PLAYER_2_CARD_3_END:
                    PD::drawBitmap(PLAYER_TWO_CARD_3_X, 0, Images::Card_Large_Bottom_Others);
                    this->state.setHandCard(PLAYER_TWO, 2, this->dealSequenceVariables.card);
                    if (this->dealSequenceVariables.startOfDeck != 0) {
                        this->dealSequenceVariables.playSeq = DEAL_BOT_TABLE_END_SEQ - 1;
                    }
                    break;
                
                case DEAL_BOT_TABLE_CARD_1_START:
                    this->dealSequenceVariables.card = this->state.drawCard();
                    this->state.addTableCard(this->dealSequenceVariables.card);
                    this->renderTable(false);
                    this->playSoundEffect(SoundTheme::Deal_02);
                    break;
                
                case DEAL_BOT_TABLE_CARD_2_START:
                    this->dealSequenceVariables.card = this->state.drawCard();
                    this->state.addTableCard(this->dealSequenceVariables.card);
                    this->renderTable(false);
                    this->playSoundEffect(SoundTheme::Deal_01);
                    break;
                
                case DEAL_BOT_TABLE_CARD_3_START:
                    this->dealSequenceVariables.card = this->state.drawCard();
                    this->state.addTableCard(this->dealSequenceVariables.card);
                    this->renderTable(false);
                    this->playSoundEffect(SoundTheme::Deal_02);
                    break;
                
                case DEAL_BOT_TABLE_CARD_4_START:
                    this->dealSequenceVariables.card = this->state.drawCard();
                    this->state.addTableCard(this->dealSequenceVariables.card);
                    this->renderTable(false);
                    this->playSoundEffect(SoundTheme::Deal_01);
                    break;

                case DEAL_BOT_TABLE_END_SEQ:
                    this->renderTopHand(PLAYER_TWO);
                    this->renderDeck();
                    this->renderPlayerName(PLAYER_ONE);
                    this->renderScore(PLAYER_ONE);
                    this->state.setGameState(GameState::Play_Hand_Init);
                    break;

                default:
                    PC::wait(20);
                    break;

            }
                    
            if (PC::buttons.pressed(BTN_A)) {

                this->state.setHandCard(PLAYER_ONE, 0, this->state.getCardFromDeck(this->dealSequenceVariables.startOfDeck));
                this->state.setHandCard(PLAYER_TWO, 0, this->state.getCardFromDeck(this->dealSequenceVariables.startOfDeck + 1));
                this->state.setHandCard(PLAYER_ONE, 1, this->state.getCardFromDeck(this->dealSequenceVariables.startOfDeck + 2));
                this->state.setHandCard(PLAYER_TWO, 1, this->state.getCardFromDeck(this->dealSequenceVariables.startOfDeck + 3));
                this->state.setHandCard(PLAYER_ONE, 2, this->state.getCardFromDeck(this->dealSequenceVariables.startOfDeck + 4));
                this->state.setHandCard(PLAYER_TWO, 2, this->state.getCardFromDeck(this->dealSequenceVariables.startOfDeck + 5));

                if (this->dealSequenceVariables.startOfDeck == 0) {

                    if (this->state.numberOfCardsOnTable() < 1) this->state.addTableCard(this->state.getCardFromDeck(this->dealSequenceVariables.startOfDeck + 6));
                    if (this->state.numberOfCardsOnTable() < 2) this->state.addTableCard(this->state.getCardFromDeck(this->dealSequenceVariables.startOfDeck + 7));
                    if (this->state.numberOfCardsOnTable() < 3) this->state.addTableCard(this->state.getCardFromDeck(this->dealSequenceVariables.startOfDeck + 8));
                    if (this->state.numberOfCardsOnTable() < 4) this->state.addTableCard(this->state.getCardFromDeck(this->dealSequenceVariables.startOfDeck + 9));

                    this->state.setDeckIndex(this->dealSequenceVariables.startOfDeck + 10);

                }
                else {

                    this->state.setDeckIndex(this->dealSequenceVariables.startOfDeck + 6);

                }

                this->renderBackground(this->getBackgroundColourByTurn());

                this->renderLowerHand(PLAYER_ONE, true);
                this->renderTopHand(PLAYER_TWO);
                this->renderTable(false);
                this->renderDeck();
                this->renderPlayerName(PLAYER_ONE);
                this->renderScore(PLAYER_ONE);
                this->state.setGameState(GameState::Play_Hand_Init);

            }

            break;

        case PLAYER_TWO:

            switch (this->state.getGameMode()) {

                case GameMode::SinglePlayer:
                        
                    switch (this->dealSequenceVariables.playSeq) {

                        case DEAL_TOP_PLAYER_2_CARD_1_START:                  
                            this->dealSequenceVariables.x = 218;
                            this->dealSequenceVariables.y = 10;
                            this->dealSequenceVariables.card = this->state.drawCard();
                            this->doPlay_Deal_Action(HandPosition::Top, 0, this->dealSequenceVariables.playSeq - DEAL_TOP_PLAYER_2_CARD_1_START, this->dealSequenceVariables.card);
                            this->playSoundEffect(SoundTheme::Deal_02);
                            break;
                            
                        case DEAL_TOP_PLAYER_2_CARD_1_START + 1 ... DEAL_TOP_PLAYER_2_CARD_1_END - 1:
                            this->renderBackground(COLOUR_P1, this->dealSequenceVariables.xOld, this->dealSequenceVariables.yOld, widths[this->dealSequenceVariables.rotOld], 98);
                            this->renderTable(false);
                            this->doPlay_Deal_Action(HandPosition::Top, 0, this->dealSequenceVariables.playSeq - DEAL_TOP_PLAYER_2_CARD_1_START, this->dealSequenceVariables.card);
                            break;
                            
                        case DEAL_TOP_PLAYER_2_CARD_1_END:
                            this->state.setHandCard(PLAYER_TWO, 0, this->dealSequenceVariables.card);
                            break;

                        case DEAL_TOP_PLAYER_1_CARD_1_START:
                            this->dealSequenceVariables.x = 220;
                            this->dealSequenceVariables.y = 70;
                            this->dealSequenceVariables.card = this->state.drawCard();
                            this->doPlay_Deal_Action(HandPosition::Bottom, 0, this->dealSequenceVariables.playSeq - DEAL_TOP_PLAYER_1_CARD_1_START, this->dealSequenceVariables.card);
                            this->playSoundEffect(SoundTheme::Deal_01);
                            break;
                            
                        case DEAL_TOP_PLAYER_1_CARD_1_START + 1 ... DEAL_TOP_PLAYER_1_CARD_1_END - 1:
                            this->renderBackground(COLOUR_P1, this->dealSequenceVariables.xOld, this->dealSequenceVariables.yOld, widths[this->dealSequenceVariables.rotOld], 98);
                            this->renderTable(false);
                            this->doPlay_Deal_Action(HandPosition::Bottom, 0, this->dealSequenceVariables.playSeq - DEAL_TOP_PLAYER_1_CARD_1_START, this->dealSequenceVariables.card);
                            break;
                            
                        case DEAL_TOP_PLAYER_1_CARD_1_END:
                            this->renderBackground(COLOUR_P1, this->dealSequenceVariables.xOld, this->dealSequenceVariables.yOld, widths[this->dealSequenceVariables.rotOld], 98);
                            this->renderTable(false);
                            this->state.setHandCard(PLAYER_ONE, 0, this->dealSequenceVariables.card);
                            this->renderLargeCard(PLAYER_ONE_CARD_1_X, PLAYER_ONE_CARD_Y, this->dealSequenceVariables.card, ROTATION_NONE);
                            break;

                        case DEAL_TOP_PLAYER_2_CARD_2_START:
                            this->dealSequenceVariables.x = 218;
                            this->dealSequenceVariables.y = 10;
                            this->dealSequenceVariables.card = this->state.drawCard();
                            this->doPlay_Deal_Action(HandPosition::Top, 1, this->dealSequenceVariables.playSeq - DEAL_TOP_PLAYER_2_CARD_2_START, this->dealSequenceVariables.card);
                            this->playSoundEffect(SoundTheme::Deal_02);
                            break;
                            
                        case DEAL_TOP_PLAYER_2_CARD_2_START + 1 ... DEAL_TOP_PLAYER_2_CARD_2_END - 1:
                            this->renderBackground(COLOUR_P1, this->dealSequenceVariables.xOld, this->dealSequenceVariables.yOld, widths[this->dealSequenceVariables.rotOld], 98);
                            this->renderTable(false);
                            this->doPlay_Deal_Action(HandPosition::Top, 1, this->dealSequenceVariables.playSeq - DEAL_TOP_PLAYER_2_CARD_2_START, this->dealSequenceVariables.card);
                            break;
                            
                        case DEAL_TOP_PLAYER_2_CARD_2_END:
                            PD::drawBitmap(PLAYER_TWO_CARD_2_X, 0, Images::Card_Large_Bottom_Others);
                            this->state.setHandCard(PLAYER_TWO, 1, this->dealSequenceVariables.card);
                            break;

                        case DEAL_TOP_PLAYER_1_CARD_2_START:
                            this->dealSequenceVariables.x = 220;
                            this->dealSequenceVariables.y = 70;
                            this->dealSequenceVariables.card = this->state.drawCard();
                            this->doPlay_Deal_Action(HandPosition::Bottom, 1, this->dealSequenceVariables.playSeq - DEAL_TOP_PLAYER_1_CARD_2_START, this->dealSequenceVariables.card);
                            this->playSoundEffect(SoundTheme::Deal_01);
                            break;
                            
                        case DEAL_TOP_PLAYER_1_CARD_2_START + 1 ... DEAL_TOP_PLAYER_1_CARD_2_END - 1:
                            this->renderBackground(COLOUR_P1, this->dealSequenceVariables.xOld, this->dealSequenceVariables.yOld, widths[this->dealSequenceVariables.rotOld], 98);
                            this->renderTable(false);
                            this->doPlay_Deal_Action(HandPosition::Bottom, 1, this->dealSequenceVariables.playSeq - DEAL_TOP_PLAYER_1_CARD_2_START, this->dealSequenceVariables.card);
                            break;
                            
                        case DEAL_TOP_PLAYER_1_CARD_2_END:  
                            this->renderBackground(COLOUR_P1, this->dealSequenceVariables.xOld, this->dealSequenceVariables.yOld, widths[this->dealSequenceVariables.rotOld], 98);
                            this->renderTable(false);
                            this->state.setHandCard(PLAYER_ONE, 1, this->dealSequenceVariables.card);
                            this->renderLargeCard(PLAYER_ONE_CARD_2_X, PLAYER_ONE_CARD_Y, this->dealSequenceVariables.card, ROTATION_NONE, true);
                            break;

                        case DEAL_TOP_PLAYER_2_CARD_3_START:
                            this->dealSequenceVariables.x = 218;
                            this->dealSequenceVariables.y = 10;
                            this->dealSequenceVariables.card = this->state.drawCard();
                            this->doPlay_Deal_Action(HandPosition::Top, 2, this->dealSequenceVariables.playSeq - DEAL_TOP_PLAYER_2_CARD_3_START, this->dealSequenceVariables.card);
                            this->playSoundEffect(SoundTheme::Deal_02);
                            break;
                            
                        case DEAL_TOP_PLAYER_2_CARD_3_START + 1 ... DEAL_TOP_PLAYER_2_CARD_3_END - 1:
                            this->renderBackground(COLOUR_P1, this->dealSequenceVariables.xOld, this->dealSequenceVariables.yOld, widths[this->dealSequenceVariables.rotOld], 98);
                            this->renderTable(false);
                            this->doPlay_Deal_Action(HandPosition::Top, 2, this->dealSequenceVariables.playSeq - DEAL_TOP_PLAYER_2_CARD_3_START, this->dealSequenceVariables.card);
                            break;
                            
                        case DEAL_TOP_PLAYER_2_CARD_3_END:
                            PD::drawBitmap(PLAYER_TWO_CARD_2_X, 0, Images::Card_Large_Bottom_Others);
                            this->state.setHandCard(PLAYER_TWO, 2, this->dealSequenceVariables.card);
                            break;

                        case DEAL_TOP_PLAYER_1_CARD_3_START:
                            this->dealSequenceVariables.x = 220;
                            this->dealSequenceVariables.y = 70;
                            this->dealSequenceVariables.card = this->state.drawCard();
                            this->doPlay_Deal_Action(HandPosition::Bottom, 2, this->dealSequenceVariables.playSeq - DEAL_TOP_PLAYER_1_CARD_3_START, this->dealSequenceVariables.card);
                            this->playSoundEffect(SoundTheme::Deal_01);
                            break;
                            
                        case DEAL_TOP_PLAYER_1_CARD_3_START + 1 ... DEAL_TOP_PLAYER_1_CARD_3_END - 1:
                            this->renderBackground(COLOUR_P1, this->dealSequenceVariables.xOld, this->dealSequenceVariables.yOld, widths[this->dealSequenceVariables.rotOld], 98);
                            this->renderTable(false);
                            this->doPlay_Deal_Action(HandPosition::Bottom, 2, this->dealSequenceVariables.playSeq - DEAL_TOP_PLAYER_1_CARD_3_START, this->dealSequenceVariables.card);
                            break;
                            
                        case DEAL_TOP_PLAYER_1_CARD_3_END:
                            this->renderBackground(COLOUR_P1, this->dealSequenceVariables.xOld, this->dealSequenceVariables.yOld, widths[this->dealSequenceVariables.rotOld], 98);
                            this->renderTable(false);
                            this->state.setHandCard(PLAYER_ONE, 2, this->dealSequenceVariables.card);
                            this->renderLargeCard(PLAYER_ONE_CARD_3_X, PLAYER_ONE_CARD_Y, this->dealSequenceVariables.card, ROTATION_NONE, true);
                            if (this->dealSequenceVariables.startOfDeck != 0) {
                                this->dealSequenceVariables.playSeq = DEAL_TOP_TABLE_END_SEQ - 1;
                            }
                            break;            
                        
                        case DEAL_TOP_TABLE_CARD_1_START:
                            this->dealSequenceVariables.card = this->state.drawCard();
                            this->state.addTableCard(this->dealSequenceVariables.card);
                            this->renderTable(false);
                            this->playSoundEffect(SoundTheme::Deal_01);
                            break;
                        
                        case DEAL_TOP_TABLE_CARD_2_START:
                            this->dealSequenceVariables.card = this->state.drawCard();
                            this->state.addTableCard(this->dealSequenceVariables.card);
                            this->renderTable(false);
                            this->playSoundEffect(SoundTheme::Deal_02);
                            break;
                        
                        case DEAL_TOP_TABLE_CARD_3_START:
                            this->dealSequenceVariables.card = this->state.drawCard();
                            this->state.addTableCard(this->dealSequenceVariables.card);
                            this->renderTable(false);
                            this->playSoundEffect(SoundTheme::Deal_01);
                            break;
                        
                        case DEAL_TOP_TABLE_CARD_4_START:
                            this->dealSequenceVariables.card = this->state.drawCard();
                            this->state.addTableCard(this->dealSequenceVariables.card);
                            this->renderTable(false);
                            this->playSoundEffect(SoundTheme::Deal_02);
                            break;

                        case DEAL_TOP_TABLE_END_SEQ:
                            this->renderTopHand(PLAYER_TWO);
                            this->renderDeck();
                            this->renderPlayerName(PLAYER_ONE);
                            this->renderScore(PLAYER_ONE);
                            this->state.setGameState(GameState::Play_Hand_Init);
                            break;

                        default:
                            PC::wait(20);
                            break;

                    }
                            
                    if (PC::buttons.pressed(BTN_A)) {

                        this->state.setHandCard(PLAYER_TWO, 0, this->state.getCardFromDeck(this->dealSequenceVariables.startOfDeck));
                        this->state.setHandCard(PLAYER_ONE, 0, this->state.getCardFromDeck(this->dealSequenceVariables.startOfDeck + 1));
                        this->state.setHandCard(PLAYER_TWO, 1, this->state.getCardFromDeck(this->dealSequenceVariables.startOfDeck + 2));
                        this->state.setHandCard(PLAYER_ONE, 1, this->state.getCardFromDeck(this->dealSequenceVariables.startOfDeck + 3));
                        this->state.setHandCard(PLAYER_TWO, 2, this->state.getCardFromDeck(this->dealSequenceVariables.startOfDeck + 4));
                        this->state.setHandCard(PLAYER_ONE, 2, this->state.getCardFromDeck(this->dealSequenceVariables.startOfDeck + 5));

                        if (this->dealSequenceVariables.startOfDeck == 0) {

                            if (this->state.numberOfCardsOnTable() < 1) this->state.addTableCard(this->state.getCardFromDeck(this->dealSequenceVariables.startOfDeck + 6));
                            if (this->state.numberOfCardsOnTable() < 2) this->state.addTableCard(this->state.getCardFromDeck(this->dealSequenceVariables.startOfDeck + 7));
                            if (this->state.numberOfCardsOnTable() < 3) this->state.addTableCard(this->state.getCardFromDeck(this->dealSequenceVariables.startOfDeck + 8));
                            if (this->state.numberOfCardsOnTable() < 4) this->state.addTableCard(this->state.getCardFromDeck(this->dealSequenceVariables.startOfDeck + 9));

                            this->state.setDeckIndex(this->dealSequenceVariables.startOfDeck + 10);

                        }
                        else {

                            this->state.setDeckIndex(this->dealSequenceVariables.startOfDeck + 6);

                        }

                        this->state.clearSelectedCard(PLAYER_ONE);
                        this->renderBackground(COLOUR_P1);
                        this->renderLowerHand(PLAYER_ONE, false);
                        this->renderTopHand(PLAYER_TWO);
                        this->renderTable(false);
                        this->renderDeck();
                        this->renderPlayerName(PLAYER_ONE);
                        this->renderScore(PLAYER_ONE);
                        this->state.setGameState(GameState::Play_Hand_Init);

                    }

                    break;

                case GameMode::PlayAndPass:
                    
                    switch (this->dealSequenceVariables.playSeq) {

                        case DEAL_BOT_PLAYER_1_CARD_1_START:
                            this->dealSequenceVariables.x = 220;
                            this->dealSequenceVariables.y = 70;
                            this->dealSequenceVariables.card = this->state.drawCard();
                            this->doPlay_Deal_Action(HandPosition::Bottom, 0, this->dealSequenceVariables.playSeq - DEAL_BOT_PLAYER_1_CARD_1_START, this->dealSequenceVariables.card);
                            this->playSoundEffect(SoundTheme::Deal_01);
                            break;
                            
                        case DEAL_BOT_PLAYER_1_CARD_1_START + 1 ... DEAL_BOT_PLAYER_1_CARD_1_END - 1:
                            this->renderBackground(COLOUR_P2, this->dealSequenceVariables.xOld, this->dealSequenceVariables.yOld, widths[this->dealSequenceVariables.rotOld], 98);
                            this->renderTable(false);
                            this->doPlay_Deal_Action(HandPosition::Bottom, 0, this->dealSequenceVariables.playSeq - DEAL_BOT_PLAYER_1_CARD_1_START, this->dealSequenceVariables.card);
                            break;
                            
                        case DEAL_BOT_PLAYER_1_CARD_1_END:
                            this->renderBackground(COLOUR_P2, this->dealSequenceVariables.xOld, this->dealSequenceVariables.yOld, widths[this->dealSequenceVariables.rotOld], 98);
                            this->renderTable(false);
                            this->state.setHandCard(PLAYER_TWO, 0, this->dealSequenceVariables.card);
                            this->renderLargeCard(PLAYER_ONE_CARD_1_X, PLAYER_ONE_CARD_Y, this->dealSequenceVariables.card, ROTATION_NONE);
                            break;

                        case DEAL_BOT_PLAYER_2_CARD_1_START:
                            this->dealSequenceVariables.x = 218;
                            this->dealSequenceVariables.y = 10;
                            this->dealSequenceVariables.card = this->state.drawCard();
                            this->doPlay_Deal_Action(HandPosition::Top, 0, this->dealSequenceVariables.playSeq - DEAL_BOT_PLAYER_2_CARD_1_START, this->dealSequenceVariables.card);
                            this->playSoundEffect(SoundTheme::Deal_02);
                            break;
                            
                        case DEAL_BOT_PLAYER_2_CARD_1_START + 1 ... DEAL_BOT_PLAYER_2_CARD_1_END - 1:
                            this->renderBackground(COLOUR_P2, this->dealSequenceVariables.xOld, this->dealSequenceVariables.yOld, widths[this->dealSequenceVariables.rotOld], 98);
                            this->renderTable(false);
                            this->doPlay_Deal_Action(HandPosition::Top, 0, this->dealSequenceVariables.playSeq - DEAL_BOT_PLAYER_2_CARD_1_START, this->dealSequenceVariables.card);
                            break;
                            
                        case DEAL_BOT_PLAYER_2_CARD_1_END:
                            this->state.setHandCard(PLAYER_ONE, 0, this->dealSequenceVariables.card);
                            break;

                        case DEAL_BOT_PLAYER_1_CARD_2_START:
                            this->dealSequenceVariables.x = 220;
                            this->dealSequenceVariables.y = 70;
                            this->dealSequenceVariables.card = this->state.drawCard();
                            this->doPlay_Deal_Action(HandPosition::Bottom, 1, this->dealSequenceVariables.playSeq - DEAL_BOT_PLAYER_1_CARD_2_START, this->dealSequenceVariables.card);
                            this->playSoundEffect(SoundTheme::Deal_01);
                            break;
                            
                        case DEAL_BOT_PLAYER_1_CARD_2_START + 1 ... DEAL_BOT_PLAYER_1_CARD_2_END - 1:
                            this->renderBackground(COLOUR_P2, this->dealSequenceVariables.xOld, this->dealSequenceVariables.yOld, widths[this->dealSequenceVariables.rotOld], 98);
                            this->renderTable(false);
                            this->doPlay_Deal_Action(HandPosition::Bottom, 1, this->dealSequenceVariables.playSeq - DEAL_BOT_PLAYER_1_CARD_2_START, this->dealSequenceVariables.card);
                            break;
                            
                        case DEAL_BOT_PLAYER_1_CARD_2_END:  
                            this->renderBackground(COLOUR_P2, this->dealSequenceVariables.xOld, this->dealSequenceVariables.yOld, widths[this->dealSequenceVariables.rotOld], 98);
                            this->renderTable(false);
                            this->state.setHandCard(PLAYER_TWO, 1, this->dealSequenceVariables.card);
                            this->renderLargeCard(PLAYER_ONE_CARD_2_X, PLAYER_ONE_CARD_Y, this->dealSequenceVariables.card, ROTATION_NONE, true);
                            break;

                        case DEAL_BOT_PLAYER_2_CARD_2_START:
                            this->dealSequenceVariables.x = 218;
                            this->dealSequenceVariables.y = 10;
                            this->dealSequenceVariables.card = this->state.drawCard();
                            this->doPlay_Deal_Action(HandPosition::Top, 1, this->dealSequenceVariables.playSeq - DEAL_BOT_PLAYER_2_CARD_2_START, this->dealSequenceVariables.card);
                            this->playSoundEffect(SoundTheme::Deal_02);
                            break;
                            
                        case DEAL_BOT_PLAYER_2_CARD_2_START + 1 ... DEAL_BOT_PLAYER_2_CARD_2_END - 1:
                            this->renderBackground(COLOUR_P2, this->dealSequenceVariables.xOld, this->dealSequenceVariables.yOld, widths[this->dealSequenceVariables.rotOld], 98);
                            this->renderTable(false);
                            this->doPlay_Deal_Action(HandPosition::Top, 1, this->dealSequenceVariables.playSeq - DEAL_BOT_PLAYER_2_CARD_2_START, this->dealSequenceVariables.card);
                            break;
                            
                        case DEAL_BOT_PLAYER_2_CARD_2_END:
                            PD::drawBitmap(PLAYER_TWO_CARD_2_X, 0, Images::Card_Large_Bottom_Others);
                            this->state.setHandCard(PLAYER_ONE, 1, this->dealSequenceVariables.card);
                            break;

                        case DEAL_BOT_PLAYER_1_CARD_3_START:
                            this->dealSequenceVariables.x = 220;
                            this->dealSequenceVariables.y = 70;
                            this->dealSequenceVariables.card = this->state.drawCard();
                            this->doPlay_Deal_Action(HandPosition::Bottom, 2, this->dealSequenceVariables.playSeq - DEAL_BOT_PLAYER_1_CARD_3_START, this->dealSequenceVariables.card);
                            this->playSoundEffect(SoundTheme::Deal_01);
                            break;
                            
                        case DEAL_BOT_PLAYER_1_CARD_3_START + 1 ... DEAL_BOT_PLAYER_1_CARD_3_END - 1:
                            this->renderBackground(COLOUR_P2, this->dealSequenceVariables.xOld, this->dealSequenceVariables.yOld, widths[this->dealSequenceVariables.rotOld], 98);
                            this->renderTable(false);
                            this->doPlay_Deal_Action(HandPosition::Bottom, 2, this->dealSequenceVariables.playSeq - DEAL_BOT_PLAYER_1_CARD_3_START, this->dealSequenceVariables.card);
                            break;
                            
                        case DEAL_BOT_PLAYER_1_CARD_3_END:
                            this->renderBackground(COLOUR_P2, this->dealSequenceVariables.xOld, this->dealSequenceVariables.yOld, widths[this->dealSequenceVariables.rotOld], 98);
                            this->renderTable(false);
                            this->state.setHandCard(PLAYER_TWO, 2, this->dealSequenceVariables.card);
                            this->renderLargeCard(PLAYER_ONE_CARD_3_X, PLAYER_ONE_CARD_Y, this->dealSequenceVariables.card, ROTATION_NONE, true);
                            break;            

                        case DEAL_BOT_PLAYER_2_CARD_3_START:
                            this->dealSequenceVariables.x = 218;
                            this->dealSequenceVariables.y = 10;
                            this->dealSequenceVariables.card = this->state.drawCard();
                            this->doPlay_Deal_Action(HandPosition::Top, 2, this->dealSequenceVariables.playSeq - DEAL_BOT_PLAYER_2_CARD_3_START, this->dealSequenceVariables.card);
                            this->playSoundEffect(SoundTheme::Deal_02);
                            break;
                            
                        case DEAL_BOT_PLAYER_2_CARD_3_START + 1 ... DEAL_BOT_PLAYER_2_CARD_3_END - 1:
                            this->renderBackground(COLOUR_P2, this->dealSequenceVariables.xOld, this->dealSequenceVariables.yOld, widths[this->dealSequenceVariables.rotOld], 98);
                            this->renderTable(false);
                            this->doPlay_Deal_Action(HandPosition::Top, 2, this->dealSequenceVariables.playSeq - DEAL_BOT_PLAYER_2_CARD_3_START, this->dealSequenceVariables.card);
                            break;
                            
                        case DEAL_BOT_PLAYER_2_CARD_3_END:
                            PD::drawBitmap(PLAYER_TWO_CARD_2_X, 0, Images::Card_Large_Bottom_Others);
                            this->state.setHandCard(PLAYER_ONE, 2, this->dealSequenceVariables.card);
                            if (this->dealSequenceVariables.startOfDeck != 0) {
                                this->dealSequenceVariables.playSeq = DEAL_BOT_TABLE_END_SEQ - 1;
                            }
                            break;
                        
                        case DEAL_BOT_TABLE_CARD_1_START:
                            this->dealSequenceVariables.card = this->state.drawCard();
                            this->state.addTableCard(this->dealSequenceVariables.card);
                            this->renderTable(false);
                            this->playSoundEffect(SoundTheme::Deal_01);
                            break;
                        
                        case DEAL_BOT_TABLE_CARD_2_START:
                            this->dealSequenceVariables.card = this->state.drawCard();
                            this->state.addTableCard(this->dealSequenceVariables.card);
                            this->renderTable(false);
                            this->playSoundEffect(SoundTheme::Deal_02);
                            break;
                        
                        case DEAL_BOT_TABLE_CARD_3_START:
                            this->dealSequenceVariables.card = this->state.drawCard();
                            this->state.addTableCard(this->dealSequenceVariables.card);
                            this->renderTable(false);
                            this->playSoundEffect(SoundTheme::Deal_01);
                            break;
                        
                        case DEAL_BOT_TABLE_CARD_4_START:
                            this->dealSequenceVariables.card = this->state.drawCard();
                            this->state.addTableCard(this->dealSequenceVariables.card);
                            this->renderTable(false);
                            this->playSoundEffect(SoundTheme::Deal_02);
                            break;

                        case DEAL_BOT_TABLE_END_SEQ:
                            this->renderTopHand(PLAYER_ONE);
                            this->renderDeck();
                            this->renderPlayerName(PLAYER_TWO);
                            this->renderScore(PLAYER_TWO);
                            this->state.setGameState(GameState::Play_Hand_Init);
                            break;

                        default:
                            PC::wait(20);
                            break;

                    }
                            
                    if (PC::buttons.pressed(BTN_A)) {

                        this->state.setHandCard(PLAYER_TWO, 0, this->state.getCardFromDeck(this->dealSequenceVariables.startOfDeck));
                        this->state.setHandCard(PLAYER_ONE, 0, this->state.getCardFromDeck(this->dealSequenceVariables.startOfDeck + 1));
                        this->state.setHandCard(PLAYER_TWO, 1, this->state.getCardFromDeck(this->dealSequenceVariables.startOfDeck + 2));
                        this->state.setHandCard(PLAYER_ONE, 1, this->state.getCardFromDeck(this->dealSequenceVariables.startOfDeck + 3));
                        this->state.setHandCard(PLAYER_TWO, 2, this->state.getCardFromDeck(this->dealSequenceVariables.startOfDeck + 4));
                        this->state.setHandCard(PLAYER_ONE, 2, this->state.getCardFromDeck(this->dealSequenceVariables.startOfDeck + 5));

                        if (this->dealSequenceVariables.startOfDeck == 0) {

                            if (this->state.numberOfCardsOnTable() < 1) this->state.addTableCard(this->state.getCardFromDeck(this->dealSequenceVariables.startOfDeck + 6));
                            if (this->state.numberOfCardsOnTable() < 2) this->state.addTableCard(this->state.getCardFromDeck(this->dealSequenceVariables.startOfDeck + 7));
                            if (this->state.numberOfCardsOnTable() < 3) this->state.addTableCard(this->state.getCardFromDeck(this->dealSequenceVariables.startOfDeck + 8));
                            if (this->state.numberOfCardsOnTable() < 4) this->state.addTableCard(this->state.getCardFromDeck(this->dealSequenceVariables.startOfDeck + 9));

                            this->state.setDeckIndex(this->dealSequenceVariables.startOfDeck + 10);

                        }
                        else {

                            this->state.setDeckIndex(this->dealSequenceVariables.startOfDeck + 6);

                        }

                        this->renderBackground(this->getBackgroundColourByTurn());

                        this->renderLowerHand(PLAYER_TWO, true);
                        this->renderTopHand(PLAYER_TWO);
                        this->renderTable(false);
                        this->renderDeck();
                        this->renderPlayerName(PLAYER_TWO);
                        this->renderScore(PLAYER_TWO);
                        this->state.setGameState(GameState::Play_Hand_Init);

                    }

                    break;


            }

            break;


    }

    this->dealSequenceVariables.playSeq++;

}



void Game::doPlay_Deal_Action(HandPosition position, uint8_t cardNumber, uint8_t seq, Card card) {

    constexpr uint8_t cardOffset[11] =  { 0, 3, 6, 10, 13, 18, 21, 24, 28, 31, 33 };
    constexpr uint8_t curveX[21] =      { 4, 4, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, };
    constexpr uint8_t curveY[21] =      { 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, };
    constexpr uint8_t stops[2][3][2] =  { 15, 26,   10, 21,   5, 16,
                                          28, 49,   23, 44,   18, 39, };

    uint8_t pos = static_cast<uint8_t>(position);
    uint8_t rotation = 9 - (seq - stops[pos][cardNumber][1]);

    switch (position) {

        case HandPosition::Bottom:

            if (seq < stops[pos][cardNumber][0]) {

                PD::drawBitmap(this->dealSequenceVariables.x, this->dealSequenceVariables.y, Images::Cards_Large_Back[0]);
                this->dealSequenceVariables.xOld = this->dealSequenceVariables.x;
                this->dealSequenceVariables.yOld = this->dealSequenceVariables.y;
                this->dealSequenceVariables.x = this->dealSequenceVariables.x - 6;
                this->dealSequenceVariables.rotOld = 0;

            }
            else if (seq >= stops[pos][cardNumber][0] && seq < stops[pos][cardNumber][1]) {

                PD::drawBitmap(this->dealSequenceVariables.x + cardOffset[seq - stops[pos][cardNumber][0]], this->dealSequenceVariables.y, Images::Cards_Large_Back[seq - stops[pos][cardNumber][0]]);
                this->dealSequenceVariables.xOld = this->dealSequenceVariables.x + cardOffset[seq - stops[pos][cardNumber][0]];
                this->dealSequenceVariables.yOld = this->dealSequenceVariables.y;
                this->dealSequenceVariables.x = this->dealSequenceVariables.x - curveX[seq - stops[pos][cardNumber][0]];
                this->dealSequenceVariables.y = this->dealSequenceVariables.y + curveY[seq - stops[pos][cardNumber][0]];
                this->dealSequenceVariables.rotOld = seq - stops[pos][cardNumber][0];
            
            }        
            else if (seq >= stops[pos][cardNumber][1]) {

                this->renderLargeCard(this->dealSequenceVariables.x + cardOffset[rotation], this->dealSequenceVariables.y, card, rotation);
                this->dealSequenceVariables.xOld = this->dealSequenceVariables.x + cardOffset[rotation];
                this->dealSequenceVariables.yOld = this->dealSequenceVariables.y;
                this->dealSequenceVariables.x = this->dealSequenceVariables.x - curveX[seq - stops[pos][cardNumber][0]];
                this->dealSequenceVariables.y = this->dealSequenceVariables.y + curveY[seq - stops[pos][cardNumber][0]];
                this->dealSequenceVariables.rotOld = rotation;

            }

            break;

        case HandPosition::Top:

            if (seq < stops[pos][cardNumber][0]) {

                PD::drawBitmap(this->dealSequenceVariables.x, this->dealSequenceVariables.y, Images::Cards_Large_Back[0]);
                this->dealSequenceVariables.xOld = this->dealSequenceVariables.x;
                this->dealSequenceVariables.yOld = this->dealSequenceVariables.y;
                this->dealSequenceVariables.x = this->dealSequenceVariables.x - 6;
                this->dealSequenceVariables.rotOld = 0;

            }
            else if (seq >= stops[pos][cardNumber][0] && seq < stops[pos][cardNumber][1]) {

                PD::drawBitmap(this->dealSequenceVariables.x, this->dealSequenceVariables.y, Images::Cards_Large_Back[0]);
                this->dealSequenceVariables.xOld = this->dealSequenceVariables.x;
                this->dealSequenceVariables.yOld = this->dealSequenceVariables.y;
                this->dealSequenceVariables.x = this->dealSequenceVariables.x - curveX[seq - stops[pos][cardNumber][0]];
                this->dealSequenceVariables.y = this->dealSequenceVariables.y - curveY[seq - stops[pos][cardNumber][0]];
                this->dealSequenceVariables.rotOld = 0;
            
            }        
            else if (seq >= stops[pos][cardNumber][1]) {

                PD::drawBitmap(this->dealSequenceVariables.x, this->dealSequenceVariables.y, Images::Cards_Large_Back[0]);
                this->dealSequenceVariables.xOld = this->dealSequenceVariables.x;
                this->dealSequenceVariables.yOld = this->dealSequenceVariables.y;
                this->dealSequenceVariables.y = this->dealSequenceVariables.y - 6;
                this->dealSequenceVariables.rotOld = 0;
            
            } 

            break;

    }

}
