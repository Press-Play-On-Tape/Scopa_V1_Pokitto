#pragma once

#include "../entities/Entities.h"
#include "Consts.h"

struct SplashScreenVariables {

    uint8_t counter = 0;
    uint8_t buttonCounter = 0;

};

struct InstructionScreenVariables {

    uint8_t counter = 0;

};

struct DealSequenceVariables {

    uint16_t playSeq = 0;
    int16_t x = 0;
    int16_t y = 0;
    Card card;
    int16_t xOld = -1;
    int16_t yOld = -1;
    uint8_t rotOld = 0;
    uint8_t startOfDeck = 0;
    uint8_t numberOfCardsOnTable = 0;

};

struct CutDeckSequenceVariables {

    uint8_t cursor = 0;
    uint8_t counter = 0;
    bool waitForSwap = false;

    void reset() {

        this->cursor = 0;
        this->counter = 0;
        this->waitForSwap = false;

    }

};

struct KeyboardSequenceVariables {

    uint8_t x = 0;
    uint8_t y = 0;
    char message[22];
    uint8_t messageIdx = 0;
    uint8_t highlightIdx = 255;
    uint8_t highlightCount = 0;
    uint8_t counter = 0;

    void reset() {

        this->x = 0;
        this->y = 0;
        this->messageIdx = 0;
        this->highlightIdx = 255;
        this->highlightCount = 0;
        this->counter = 0;

        for (uint8_t i = 0; i < 22; i++) this->message[i] = 0;

    };

};

struct ScoreSequenceVariables {

    uint8_t playerOne_Carte = 0;
    uint8_t playerTwo_Carte = 0;

    uint8_t playerOne_Denari = 0;
    uint8_t playerTwo_Denari = 0;

    uint8_t playerOne_SetteBello = 0;
    uint8_t playerTwo_SetteBello = 0;

    uint8_t playerOne_Premiera = 0;
    uint8_t playerTwo_Premiera = 0;

    uint8_t winner = NO_WINNER;
    uint8_t counter = 0;

    uint8_t playerOne_Score() {

        return playerOne_Carte + playerOne_Denari + playerOne_SetteBello + playerOne_Premiera;

    }

    uint8_t playerTwo_Score() {

        return playerTwo_Carte + playerTwo_Denari + playerTwo_SetteBello + playerTwo_Premiera;

    }

    void reset() {

        this->playerOne_Carte = 0;
        this->playerTwo_Carte = 0;

        this->playerOne_Denari = 0;
        this->playerTwo_Denari = 0;

        this->playerOne_SetteBello = 0;
        this->playerTwo_SetteBello = 0;

        this->playerOne_Premiera = 0;
        this->playerTwo_Premiera = 0;

        this->winner = NO_WINNER;
        this->counter = 0;

    }

};

struct TitleSequenceVariables {

    uint8_t delay = 20;
    uint16_t star = 0;
    uint8_t colour = 0;
    uint8_t coinFrame = 0;
    uint8_t spinCounter = 0;
    uint8_t menuOption = 0;
    TitleSequenceMode mode = TitleSequenceMode::Normal;

    void reset() {

        this->delay = 20;
        this->star = 0;
        this->colour = 0;
        this->coinFrame = 0;
        this->spinCounter = 0;
        this->menuOption = 0;
        this->mode = TitleSequenceMode::Normal;

    }

};

