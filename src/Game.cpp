#include "Pokitto.h"
#include "Game.h"
#include "images/Images.h"
#include "entities/Entities.h"
#include "images/Panel.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;


void Game::setup(GameCookie *cookie) {

	this->cookie = cookie;

}


void Game::loop(){

    PC::buttons.pollButtons();


    // Game state ..

    switch (this->state.getGameState()) {

        case GameState::SplashScreen_Init:
            this->doSplashScreen_Init(); 
            [[fallthrough]]

        case GameState::SplashScreen:
            PD::clear();
            this->doSplashScreen(); 
            PD::update();
            break;

        case GameState::Title_Init:
            this->doTitle_Init(); 
            PD::update();
            [[fallthrough]]

        case GameState::Title:
            this->doTitle(); 
            break;

        case GameState::Play_Init:
            PC::buttons.pollButtons();
            this->state.setGameState(GameState::Play_CutDeck_Init);
            this->renderBackground(this->getBackgroundColourByTurn());
            [[fallthrough]]

        case GameState::Play_CutDeck_Init:
            doPlay_CutDeck_Init();
            [[fallthrough]]

        case GameState::Play_CutDeck:
            PC::buttons.pollButtons();
            doPlay_CutDeck();
            PD::update();
            break;

        case GameState::Play_Deal_Init:
            doPlay_Deal_Init();
            [[fallthrough]]

        case GameState::Play_Deal:
            PC::buttons.pollButtons();
            doPlay_Deal();
            PD::update();
            break;

        case GameState::Play_Hand_Init:
            doPlay_Hand_Init();
            [[fallthrough]]

        case GameState::Play_Hand:
            PC::buttons.pollButtons();
            doPlay_Hand();
            break;

        case GameState::Play_AddCardToHand:
            doPlay_AddToTable();            
            break;

        case GameState::Play_TakeMatch:
            doPlay_TakeMatch();            
            break;

        case GameState::Play_NominateDeckCards_Single:
        case GameState::Play_NominateDeckCards_Multiple:
            doPlay_NominateDeckCards();            
            break;
            
        case GameState::Play_SwapPlayers_Init:
            doPlay_SwapPlayers_Init();
            break;
            
        case GameState::Play_SwapPlayers:
            doPlay_SwapPlayers();
            break;
            
        case GameState::Play_Scopa_Init:
            doPlay_Scopa_Init();
            [[fallthrough]]
            
        case GameState::Play_Scopa:
            doPlay_Scopa();
            break;
            
        case GameState::Play_Score_Init:
            doPlay_Score_Init();
            PD::update();
            [[fallthrough]]
            
        case GameState::Play_Score:
            doPlay_Score();
            break;
            
        case GameState::Play_Keyboard_Init:
            doPlay_Keyboard_Init(KeyboardMode::Message);
            [[fallthrough]]
            
        case GameState::Play_Keyboard:
            doPlay_Keyboard(KeyboardMode::Message);
            break;
            
        case GameState::Play_PlayerOneName_Init:
            doPlay_Keyboard_Init(KeyboardMode::PlayerOneName);
            [[fallthrough]]
            
        case GameState::Play_PlayerOneName:
            doPlay_Keyboard(KeyboardMode::PlayerOneName);
            break;
            
        case GameState::Play_PlayerTwoName_Init:
            doPlay_Keyboard_Init(KeyboardMode::PlayerTwoName);
            [[fallthrough]]
            
        case GameState::Play_PlayerTwoName:
            doPlay_Keyboard(KeyboardMode::PlayerTwoName);
            break;
            
        case GameState::Play_LastCards_Init:
            doPlay_LastCards_Init();
            PD::update();
            [[fallthrough]]
            
        case GameState::Play_LastCards:
            doPlay_LastCards();
            break;
            
        case GameState::Instructions_Init:
            doInstructions_Init();
            PD::update();
            [[fallthrough]]
            
        case GameState::Instructions:
            doInstructions();
            break;

    }

}
