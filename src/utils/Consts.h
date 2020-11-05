#pragma once

#define SOUNDS
#define _DEBUG
#define _DEBUG_PLAYER_ONE_PLAYS_FIRST
#define _DEBUG_PLAYER_TWO_PLAYS_FIRST

#define COOKIE_INITIALISED 16
#define PLAYER_ONE 0
#define PLAYER_TWO 1
#define DRAW 2
#define NO_WINNER 3
#define ROTATION_NONE 0
#define NO_CARD 40
#define NO_SELECTION 255

// #define COLOUR_P1 2
// #define COLOUR_P1_ALT 4
// #define COLOUR_P2 1
// #define COLOUR_P2_ALT 12
#define COLOUR_P1 1
#define COLOUR_P1_ALT 13
#define COLOUR_P2 255
#define COLOUR_P2_ALT 12

#define GAME_OVER_DELAY 140

#define SUIT_COINS 0
#define SUIT_VASE  10
#define SUIT_CLUBS 20
#define SUIT_SWORDS 30
#define CARD_ONE 0
#define CARD_TWO 1
#define CARD_THREE 2
#define CARD_FOUR 3
#define CARD_FIVE 4
#define CARD_SIX 5
#define CARD_SEVEN 6
#define CARD_KNAVE 7
#define CARD_HORSE 8
#define CARD_KING 9

constexpr uint32_t SDCardLength                       = (20 + (390 * 98 / 2));

constexpr uint32_t SDTitleStart = (SDCardLength * 40);
constexpr uint32_t SDTitleLength = 2 + (220 * 176 / 2);
constexpr uint32_t SDScoreStart = SDTitleStart + SDTitleLength;
constexpr uint32_t SDScoreLength = 2 + (220 * 176 / 2);
constexpr uint32_t SDPpotStart = SDScoreStart + SDScoreLength;
constexpr uint32_t SDPpotLength = 2 + (132 * 68 / 2);
constexpr uint32_t SDTitleReplaceStart = SDPpotStart + SDPpotLength;
constexpr uint32_t SDTitleReplaceLength = 2 + (162 * 62 / 2);
constexpr uint32_t SDInstruction01Start = SDTitleReplaceStart + SDTitleReplaceLength;
constexpr uint32_t SDInstruction01Length = 2 + (220 * 176 / 2);
constexpr uint32_t SDInstruction02Start = SDInstruction01Start + SDInstruction01Length;
constexpr uint32_t SDInstruction02Length = 2 + (220 * 176 / 2);
constexpr uint32_t SDInstruction03Start = SDInstruction02Start + SDInstruction02Length;
constexpr uint32_t SDInstruction03Length = 2 + (220 * 176 / 2);
constexpr uint32_t SDInstruction04Start = SDInstruction03Start + SDInstruction03Length;
constexpr uint32_t SDInstruction04Length = 2 + (220 * 176 / 2);
constexpr uint32_t SDInstruction05Start = SDInstruction04Start + SDInstruction04Length;
constexpr uint32_t SDInstruction05Length = 2 + (220 * 176 / 2);
constexpr uint32_t SDInstruction06Start = SDInstruction05Start + SDInstruction05Length;
constexpr uint32_t SDInstruction06Length = 2 + (220 * 176 / 2);
constexpr uint32_t SDInstruction07Start = SDInstruction06Start + SDInstruction06Length;
constexpr uint32_t SDInstruction07Length = 2 + (220 * 176 / 2);
constexpr uint32_t SDInstruction08Start = SDInstruction07Start + SDInstruction07Length;
constexpr uint32_t SDInstruction08Length = 2 + (220 * 176 / 2);
constexpr uint32_t SDInstruction09Start = SDInstruction08Start + SDInstruction08Length;
constexpr uint32_t SDInstruction09Length = 2 + (220 * 176 / 2);
constexpr uint32_t SDInstruction10Start = SDInstruction09Start + SDInstruction09Length;
constexpr uint32_t SDInstruction10Length = 2 + (220 * 176 / 2);
constexpr uint32_t SDInstruction11Start = SDInstruction10Start + SDInstruction10Length;
constexpr uint32_t SDInstruction11Length = 2 + (220 * 176 / 2);

enum class TitleSequenceMode : uint8_t {
    Normal,
    Menu,
    PlayAndPass
};

enum class KeyboardMode : uint8_t {
    Message,
    PlayerOneName,
    PlayerTwoName
};

enum class GameMode : uint8_t {
    SinglePlayer,
    PlayAndPass,
    TwoPlayers
};

enum class PanelMessage : uint8_t {
    SwapPlayers,
    PickACard_PlayerOne,
    PickACard_PlayerTwo,   
    PickACard_Human,
    PickACard_Computer,   
    PlayerOneWins,
    PlayerTwoWins,
    HumanWins,
    PlayerComputerWins,
    Draw,
    LastCards_PlayerOne,
    LastCards_PlayerTwo,
    LastCards_Human,
    LastCards_Computer,
};

enum class SoundTheme : uint8_t {
    Deal_01,
    Deal_02,
    Scopa,
    Coins   
};

enum class PointerPosition : uint8_t {
    Hand,
    Table   
};

enum class HandPosition : uint8_t {
    Bottom,   
    Top,
};

enum class CardStatus : uint8_t {
    InDeck,
    PlayerOneHand,
    PlayerTwoHand,
    OnTable,
};

enum class GameState : uint8_t {
    SplashScreen_Init,
    SplashScreen,
    Title_Init,
    Title,
    Play_Init,
    Play_Deal_Init,
    Play_Deal,
    Play_Hand_Init,
    Play_Hand,
    Play_TurnTwo,
    Play_AddCardToHand,
    Play_TakeMatch,
    Play_NominateDeckCards_Single,
    Play_NominateDeckCards_Multiple,
    Play_SwapPlayers_Init,
    Play_SwapPlayers,
    Play_Scopa_Init,
    Play_Scopa,
    Play_Score_Init,
    Play_Score,
    Play_CutDeck_Init,
    Play_CutDeck,
    Play_Keyboard_Init,
    Play_Keyboard,
    Play_LastCards_Init,
    Play_LastCards,
    Play_PlayerOneName_Init,
    Play_PlayerOneName,
    Play_PlayerTwoName_Init,
    Play_PlayerTwoName,
    Instructions_Init,
    Instructions,
    End,
};



constexpr int16_t PLAYER_CARD_SPACING                 = 30;
constexpr int16_t PLAYER_CARD_WIDTH                   = 70;
constexpr int16_t PLAYER_CARD_HEIGHT                  = 90;
constexpr int16_t PLAYER_ONE_CARD_1_X                 = 84; 
constexpr int16_t PLAYER_ONE_CARD_2_X                 = PLAYER_ONE_CARD_1_X + PLAYER_CARD_SPACING;
constexpr int16_t PLAYER_ONE_CARD_3_X                 = PLAYER_ONE_CARD_2_X + PLAYER_CARD_SPACING;
constexpr int16_t PLAYER_ONE_CARD_1H_X                = PLAYER_ONE_CARD_1_X + (PLAYER_CARD_SPACING / 2);
constexpr int16_t PLAYER_ONE_CARD_2H_X                = PLAYER_ONE_CARD_2_X + (PLAYER_CARD_SPACING / 2);

constexpr int16_t PLAYER_TWO_CARD_1_X                 = 6;
constexpr int16_t PLAYER_TWO_CARD_2_X                 = PLAYER_TWO_CARD_1_X + PLAYER_CARD_SPACING;
constexpr int16_t PLAYER_TWO_CARD_3_X                 = PLAYER_TWO_CARD_2_X + PLAYER_CARD_SPACING;
constexpr int16_t PLAYER_TWO_CARD_1H_X                = PLAYER_TWO_CARD_1_X + (PLAYER_CARD_SPACING / 2);
constexpr int16_t PLAYER_TWO_CARD_2H_X                = PLAYER_TWO_CARD_2_X + (PLAYER_CARD_SPACING / 2);

constexpr int16_t PLAYER_ONE_CARD_Y                   = 116;
constexpr int16_t PLAYER_TWO_CARD_Y                   = -84;

constexpr uint16_t DEAL_BOT_PLAYER_1_CARD_1_START     = 0;
constexpr uint16_t DEAL_BOT_PLAYER_1_CARD_1_END       = DEAL_BOT_PLAYER_1_CARD_1_START + 36;
constexpr uint16_t DEAL_BOT_PLAYER_2_CARD_1_START     = DEAL_BOT_PLAYER_1_CARD_1_END + 1;
constexpr uint16_t DEAL_BOT_PLAYER_2_CARD_1_END       = DEAL_BOT_PLAYER_2_CARD_1_START + 59;
constexpr uint16_t DEAL_BOT_PLAYER_1_CARD_2_START     = DEAL_BOT_PLAYER_2_CARD_1_END + 1;
constexpr uint16_t DEAL_BOT_PLAYER_1_CARD_2_END       = DEAL_BOT_PLAYER_1_CARD_2_START + 31;
constexpr uint16_t DEAL_BOT_PLAYER_2_CARD_2_START     = DEAL_BOT_PLAYER_1_CARD_2_END + 1;
constexpr uint16_t DEAL_BOT_PLAYER_2_CARD_2_END       = DEAL_BOT_PLAYER_2_CARD_2_START + 54;
constexpr uint16_t DEAL_BOT_PLAYER_1_CARD_3_START     = DEAL_BOT_PLAYER_2_CARD_2_END + 1;
constexpr uint16_t DEAL_BOT_PLAYER_1_CARD_3_END       = DEAL_BOT_PLAYER_1_CARD_3_START + 26;
constexpr uint16_t DEAL_BOT_PLAYER_2_CARD_3_START     = DEAL_BOT_PLAYER_1_CARD_3_END + 1;
constexpr uint16_t DEAL_BOT_PLAYER_2_CARD_3_END       = DEAL_BOT_PLAYER_2_CARD_3_START + 49;
constexpr uint16_t DEAL_BOT_TABLE_CARD_1_START        = DEAL_BOT_PLAYER_2_CARD_3_END + 1;
constexpr uint16_t DEAL_BOT_TABLE_CARD_2_START        = DEAL_BOT_TABLE_CARD_1_START + 20;
constexpr uint16_t DEAL_BOT_TABLE_CARD_3_START        = DEAL_BOT_TABLE_CARD_2_START + 20;
constexpr uint16_t DEAL_BOT_TABLE_CARD_4_START        = DEAL_BOT_TABLE_CARD_3_START + 20;
constexpr uint16_t DEAL_BOT_TABLE_END_SEQ             = DEAL_BOT_TABLE_CARD_4_START + 1;

constexpr uint16_t DEAL_TOP_PLAYER_2_CARD_1_START     = 0;
constexpr uint16_t DEAL_TOP_PLAYER_2_CARD_1_END       = DEAL_TOP_PLAYER_2_CARD_1_START + 59;
constexpr uint16_t DEAL_TOP_PLAYER_1_CARD_1_START     = DEAL_TOP_PLAYER_2_CARD_1_END + 1;
constexpr uint16_t DEAL_TOP_PLAYER_1_CARD_1_END       = DEAL_TOP_PLAYER_1_CARD_1_START + 36;
constexpr uint16_t DEAL_TOP_PLAYER_2_CARD_2_START     = DEAL_TOP_PLAYER_1_CARD_1_END + 1;
constexpr uint16_t DEAL_TOP_PLAYER_2_CARD_2_END       = DEAL_TOP_PLAYER_2_CARD_2_START + 54;
constexpr uint16_t DEAL_TOP_PLAYER_1_CARD_2_START     = DEAL_TOP_PLAYER_2_CARD_2_END + 1;
constexpr uint16_t DEAL_TOP_PLAYER_1_CARD_2_END       = DEAL_TOP_PLAYER_1_CARD_2_START + 31;
constexpr uint16_t DEAL_TOP_PLAYER_2_CARD_3_START     = DEAL_TOP_PLAYER_1_CARD_2_END + 1;
constexpr uint16_t DEAL_TOP_PLAYER_2_CARD_3_END       = DEAL_TOP_PLAYER_2_CARD_3_START + 49;
constexpr uint16_t DEAL_TOP_PLAYER_1_CARD_3_START     = DEAL_TOP_PLAYER_2_CARD_3_END + 1;
constexpr uint16_t DEAL_TOP_PLAYER_1_CARD_3_END       = DEAL_TOP_PLAYER_1_CARD_3_START + 26;
constexpr uint16_t DEAL_TOP_TABLE_CARD_1_START        = DEAL_TOP_PLAYER_1_CARD_3_END + 1;
constexpr uint16_t DEAL_TOP_TABLE_CARD_2_START        = DEAL_TOP_TABLE_CARD_1_START + 20;
constexpr uint16_t DEAL_TOP_TABLE_CARD_3_START        = DEAL_TOP_TABLE_CARD_2_START + 20;
constexpr uint16_t DEAL_TOP_TABLE_CARD_4_START        = DEAL_TOP_TABLE_CARD_3_START + 20;
constexpr uint16_t DEAL_TOP_TABLE_END_SEQ             = DEAL_TOP_TABLE_CARD_4_START + 1;

constexpr uint8_t KEY_SMILEY_FACE = 26;
constexpr uint8_t KEY_FROWN_FACE = 27;
constexpr uint8_t KEY_START = 0;
constexpr uint8_t KEY_END = 25;
constexpr uint8_t KEY_ENTER = 28;
constexpr uint8_t KEY_DELETE = 29;
constexpr uint8_t KEY_SPACE = 30;
constexpr uint8_t KEY_PERIOD = 31;
constexpr uint8_t ASCII_PERIOD = 46;
constexpr uint8_t ASCII_SPACE = 32;
constexpr uint8_t ASCII_START = 65;
constexpr uint8_t ASCII_END = 90;