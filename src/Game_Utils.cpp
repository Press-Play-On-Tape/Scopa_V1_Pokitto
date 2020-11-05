#include "Pokitto.h"
#include "Game.h"
#include "utils/Consts.h"
#include "images/Images.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;


void Game::playSoundEffect(SoundTheme theme) {

    #ifdef SOUNDS

    char sounds[5][19] = { "music/SCOP_D01.raw", "music/SCOP_D02.raw", "music/SCOP_D03.raw", "music/SCOP_D04.raw","music/SCOP_D05.raw" };

    if (soundEffectFile.openRO(sounds[static_cast<uint8_t>(theme)])) {

        auto& music = Audio::play<1>(soundEffectFile);
        music.setLoop(false);

    }

    #endif

}


void Game::playTheme(uint8_t trackNumber) {

    #ifdef SOUNDS

    char themes[1][19] = { "music/SCOP_08K.raw" };

    if (mainThemeFile.openRO(themes[trackNumber])) {
        auto& music = Audio::play<0>(mainThemeFile);
        music.setLoop(true);
    }

    #endif

}



uint8_t Game::getBackgroundColourByTurn() {

    uint8_t colour = COLOUR_P1;

    switch (this->state.getGameMode()) {

        case GameMode::SinglePlayer:
            break;

        case GameMode::PlayAndPass:
            colour = (this->state.getCurrentTurn() == PLAYER_ONE ? COLOUR_P1 : COLOUR_P2);
            break;

    }

    return colour == 255 ? COLOUR_P1 : colour;

}

uint8_t Game::getBackgroundColourByDeal() {

    uint8_t colour = COLOUR_P1;

    switch (this->state.getGameMode()) {

        case GameMode::SinglePlayer:
            break;

        case GameMode::PlayAndPass:
            colour = (this->state.getCurrentTurn() == PLAYER_ONE ? COLOUR_P1 : COLOUR_P2);
            break;

    }

    return colour == 255 ? COLOUR_P1 : colour;

}
