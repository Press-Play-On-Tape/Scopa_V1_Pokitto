#pragma once

#include "Pokitto.h"
#include <LibAudio>
#include "utils/GameCookie.h"
#include "entities/Entities.h"
#include "utils/Structs.h"

using PC=Pokitto::Core;
using PD=Pokitto::Display;


class Game {
    
    private:

        File file;
        State state; 

        SplashScreenVariables splashScreenVariables;
        DealSequenceVariables dealSequenceVariables;
        ScoreSequenceVariables scoreSequenceVariables;
        CutDeckSequenceVariables cutDeckSequenceVariables;
        TitleSequenceVariables titleSequenceVariables;
        InstructionScreenVariables instructionScreenVariables;

        File mainThemeFile;
        File soundEffectFile;

        GameCookie *cookie;

        // uint8_t imgBuffer[2 + (70 * 98 / 2)];
        //uint8_t imgBuffer[2 + (131 * 68 / 2)];
        uint8_t imgBuffer[2 + (162 * 62 / 2)];
        uint8_t counter = 0;
        bool check[20];

    public:

        void setup(GameCookie *cookie);
        void loop();

    private:


        // Game state handlers ..

        void doSplashScreen_Init();
        void doSplashScreen();
        void doTitle_Init();
        void doTitle();
        void doPlay_Hand_Init();
        void doPlay_Hand();
        void doPlay_Hand_HandleActions(KeyboardSequenceVariables keyboardSequenceVariables, uint8_t currentPlayer);
        void doPlay_Hand_ComputerAction();
        void doPlay_Deal_Init();
        void doPlay_Deal();
        void doPlay_Deal_Action(HandPosition position, uint8_t cardNumber, uint8_t seq, Card card);
        void doPlay_AddToTable();
        void doPlay_TakeMatch();
        void doPlay_TakeMatch_IsCoin(uint8_t currentPlayer, uint8_t selectedIdx);
        void doPlay_SwapPlayers_Init();
        void doPlay_SwapPlayers();
        void doPlay_SwapPlayers_RenderPanel(uint8_t player, int8_t yOffset);
        void doPlay_Scopa_Init();
        void doPlay_Scopa();
        void doPlay_Score_Init();
        void doPlay_Score_RenderPanel(uint8_t player);
        void doPlay_Score();
        void doPlay_CutDeck_Init();
        void doPlay_CutDeck();
        void doPlay_CutDeck_RenderCards();
        void doPlay_CutDeck_RenderPanel(uint8_t player);
        void doPlay_Keyboard_Init(KeyboardMode keyboardMode);
        void doPlay_Keyboard(KeyboardMode keyboardMode);
        void doPlay_Keyboard_Render();
        void doPlay_Keyboard_RenderKey(char charToRender, uint8_t x);
        void doPlay_Keyboard_Exit(KeyboardMode mode, KeyboardSequenceVariables &keyboardSequenceVariables);
        void doPlay_NominateDeckCards();
        void doPlay_Score_Line(uint8_t y, uint8_t score1, uint8_t score2, bool flashScore1, bool flashScore2);
        void doPlay_LastCards_Init();
        void doPlay_LastCards();
        void doPlay_LastCards_RenderPanel(uint8_t player);
        void doInstructions_Init();
        void doInstructions();


        // Game play ..

        void readImage(uint8_t *buffer, Card card, uint8_t rotation, uint8_t height);
        void renderLargeCard(uint8_t x, int16_t y, Card card, uint8_t rotation, bool topCorner = false);
        void renderLargeCard_Top(uint8_t x, int16_t y, Card card, uint8_t rotation, bool topCorner = false);
        void renderSmallCard(uint8_t x, uint8_t y, Card card, bool highlight = false);
        void renderTopHand(uint8_t player, bool showCard = false, bool highlight = false);
        void renderTopHand_Helper(uint8_t player, uint8_t cardNo, uint8_t selectedCard, int16_t x, bool showCard, bool highlight, bool omitLowerCorner);
        void renderLowerHand(uint8_t player, bool highlight = true, bool hide = false);
        void renderLowerHand_Helper(uint8_t player, uint8_t cardNo, int16_t x, int16_t y, bool highlight, bool hide, bool topCorner);      
        void renderPointerHand(uint8_t player, PointerPosition position);
        void renderTable(bool highlight);
        void renderDeck();
        void renderBackground(uint8_t colour, int16_t x = 255, int16_t y = 255, uint16_t w = 255, uint16_t h = 255);
        void renderPlayerName(uint8_t botPlayer);
        void renderScore(uint8_t botPlayer);
        void renderScore_Individual(uint8_t x, uint8_t y, uint8_t score);
        void renderPanel(PanelMessage panelMessage, int8_t yOffset = 0);
        void renderPanelMessage(char message[], int8_t xOffset, int8_t yOffset);
        void renderString(char message[], uint8_t messageLen, uint8_t x, uint8_t y);


        void debugHands();
        void fillRectAlt(int x0,int y0, int w, int h, uint8_t colour1, uint8_t colour2);


        // Utilities ..

        void playTheme(uint8_t trackNumber);
        void playSoundEffect(SoundTheme theme);

        void Combi(Card cardToMatch, uint8_t &matches, uint8_t matchArray[], uint8_t a[], uint8_t reqLen, uint8_t s, uint8_t currLen, bool check[], uint8_t l);
        uint8_t getBackgroundColourByTurn();
        uint8_t getBackgroundColourByDeal();
        void calculateScores();
};


