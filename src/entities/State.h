#include "Pokitto.h"
#include "../utils/Consts.h"
#include "../utils/Utils.h"
#include "../utils/Structs.h"

struct State {

    private:

        CardStatus deck[40];
        GameState gameState = GameState::SplashScreen_Init; 

        Card playerCards[2][3];
        Card table[20];
        
        uint8_t seq[40];
        uint8_t deckIdx;
        uint8_t tableIdx;        

        uint8_t pointer[2][2] = { 0, 0, 0, 0 };   
        uint8_t selectedCard[2] = { NO_SELECTION, NO_SELECTION };   
        bool selectedTable[20] = { };   
        uint8_t currentTurn = 0;
        uint8_t currentDeal = 1;
        uint8_t scopa[2] = { 0, 0 };
        uint8_t score[2] = { 0, 0 };
        uint8_t lastScore = PLAYER_ONE;

        KeyboardSequenceVariables keyboardSequenceVariables[3];  // Third one for altering names ..
        GameMode gameMode = GameMode::PlayAndPass;

    public:

        GameState getGameState()                        { return this->gameState; }
        GameMode getGameMode()                          { return this->gameMode; }
        uint8_t getCurrentTurn()                        { return this->currentTurn; }
        uint8_t getCurrentDeal()                        { return this->currentDeal; }
        uint8_t getDeckIndex()                          { return this->deckIdx; }
        uint8_t getLastScore()                          { return this->lastScore; }
        
        void setGameState(GameState val)                { this->gameState = val; }
        void setGameMode(GameMode val)                  { this->gameMode = val; }
        void setCurrentTurn(uint8_t val)                { this->currentTurn = val; }
        void setCurrentDeal(uint8_t val)                { this->currentDeal = val; this->currentTurn = val; }
        void setDeckIndex(uint8_t index)                { this->deckIdx = index; }

    public:

        KeyboardSequenceVariables &getKeyboardSequenceVariables(uint8_t player) { 
        
            KeyboardSequenceVariables &keyboardVars = this->keyboardSequenceVariables[player]; 
            return keyboardVars; 
            
        }

        uint8_t getScore(uint8_t player) { 
        
            return this->score[player]; 
            
        }

        void setScore(uint8_t player, uint8_t score) { 
        
            this->score[player] = score; 
            
        }

        uint8_t getScopa(uint8_t player) { 
        
            return this->scopa[player]; 
            
        }

        void incScopa(uint8_t player) { 

            this->scopa[player] = this->scopa[player] + 1; 

        }

        void setScopa(uint8_t player, uint8_t value) { 
        
            this->scopa[player] = value; 
            
        }

        uint8_t getPointer(uint8_t player, PointerPosition position) { 
        
            return this->pointer[player][static_cast<uint8_t>(position)]; 
            
        }

        void setPointer(uint8_t player, PointerPosition position, uint8_t idx) { 
        
            this->pointer[player][static_cast<uint8_t>(position)] = idx; 
            
        }

        uint8_t getSelectedCard(uint8_t player) { 
        
            return this->selectedCard[player]; 
            
        }

        void setSelectedCard(uint8_t player) { 

            this->selectedCard[player] = this->pointer[player][static_cast<uint8_t>(PointerPosition::Hand)]; 
            
        }

        void setSelectedCard(uint8_t player, uint8_t idx) { 
        
            this->selectedCard[player] = idx; 
            
        }

        void clearSelectedCard(uint8_t player) {

            this->selectedCard[player] = NO_SELECTION; 
            this->pointer[player][static_cast<uint8_t>(PointerPosition::Hand)] = 255; 
            this->pointer[player][static_cast<uint8_t>(PointerPosition::Table)] = 255; 
            
        }


        void setHandCard(uint8_t player, uint8_t cardNumber, Card card) {

            playerCards[player][cardNumber] = card;

        }

        Card& getHandCard(uint8_t player, uint8_t cardNumber) {

            return playerCards[player][cardNumber];

        }

        uint8_t numberOfCardsOnTable() {

            return this->tableIdx;

        }

        Card& getTableCard(uint8_t cardNumber) {

            return this->table[cardNumber];

        }

        Card getCardFromDeck(uint8_t cardNumber) {
            
            Card card;
            card.setCardIdx(seq[cardNumber]);
            return card;

        }

        void removeTableCard(uint8_t player, uint8_t cardNumber, bool addToPlayerScore) {

            Card card = this->table[cardNumber];

            for (uint8_t i = cardNumber; i < 19; i++) {

                this->table[i] = this->table[i + 1];

            }

            this->table[19].setCardIdx(NO_CARD);
            this->tableIdx--;

            if (addToPlayerScore) {

                this->deck[card.getCardIdx()] = (player == PLAYER_ONE ? CardStatus::PlayerOneHand : CardStatus::PlayerTwoHand);
                this->lastScore = player;

            }

        }

        void removeTableCards(uint8_t player, bool addToPlayerScore) {
            
            for (uint8_t i = 20; i > 0; i--) {

                Card card = this->table[i - 1];

                if (this->selectedTable[i - 1]) {

                    removeTableCard(player, i - 1, addToPlayerScore);

                    if (addToPlayerScore) {

                        this->deck[card.getCardIdx()] = (player == PLAYER_ONE ? CardStatus::PlayerOneHand : CardStatus::PlayerTwoHand);
                        this->lastScore = player;

                    }

                }

            }

        }

        uint8_t numberOfCardsInHand(uint8_t player) {

            for (uint8_t x = 3; x > 0; x--) {

                if (this->playerCards[player][x - 1].getSuit() != 4) {

                    return x;

                }

            }

            return 0;

        }

        void reset() {

            this->score[0] = 0;
            this->score[1] = 0;

            this->keyboardSequenceVariables[0].reset();
            this->keyboardSequenceVariables[1].reset();

            this->selectedCard[0] = NO_SELECTION;
            this->selectedCard[1] = NO_SELECTION;

        }

        void shuffle(bool singleSuit) {


            // Reset the deck ..

            for (uint8_t x = 0; x < 40; x++) {

                this->deck[x] = CardStatus::InDeck;
                this->seq[x] = x;
                
            }


            // Rest player hands ..

            for (uint8_t x = 0; x < 3; x++) {

                Card card;
                card.setCardIdx(NO_CARD);

                this->playerCards[0][x] = card;
                this->playerCards[1][x] = card;

            }


            // Clear table ..

            for (uint8_t x = 0; x < 20; x++) {

                Card card;
                card.setCardIdx(NO_CARD);

                this->table[x] = card;
                this->selectedTable[x] = false;

            }

            this->tableIdx = 0;


            // Shuffle the deck ..

            for (uint8_t x = 0; x < 50; x++) {

                if (singleSuit) {

                    uint8_t one = random(0, 10);
                    uint8_t two = random(0, 10);
                    uint8_t temp = this->seq[one];

                    this->seq[one] = this->seq[two];
                    this->seq[two] = temp;

                }
                else {

                    uint8_t one = random(0, 40);
                    uint8_t two = random(0, 40);
                    uint8_t temp = this->seq[one];

                    this->seq[one] = this->seq[two];
                    this->seq[two] = temp;

                }

            }

            this->scopa[0] = 0;
            this->scopa[1] = 0;
            this->deckIdx = 0;






            // this->seq[0] = 0;
            // this->seq[2] = 1;

            // this->seq[0] = 9;
            // this->seq[2] = 8;
            // this->seq[4] = 35;
            // this->seq[6] = 12;
            // this->seq[7] = 15;
            // this->seq[8] = 25;
            // this->seq[9] = 26;
            // this->seq[6] = 0;
            // this->seq[7] = 10;
            // this->seq[8] = 23;
            // this->seq[9] = 33;

            // this->deckIdx = 0;
            // this->selectedCard[0] = 255;
            // this->selectedCard[1] = 255;



            // this->seq[0] = 39;
            // this->seq[2] = 7;
            // this->seq[4] = 35;
            // this->seq[1] = 19;
            // this->seq[3] = 26;
            // this->seq[5] = 35;
            // this->seq[6] = 0;
            // this->seq[7] = 8;
            // this->seq[8] = 9;
            // this->seq[9] = 33;



            #ifdef DEBUG
            printf("Deck ");
            for (uint8_t x = 0; x < 40; x++) {
                printf("%i ", this->seq[x]);
            }
            printf("\n");
            #endif

        }


        Card drawCard() {

            if (deckIdx < 40) {
                
                Card card;
                card.setCardIdx(seq[this->deckIdx++]);
                return card;

            }
            else {

                Card card;
                card.setCardIdx(NO_CARD);
                return card;

            }

        }

        void addTableCard(Card card) {

            if (this->tableIdx < 18) {

                this->table[tableIdx++] = card;

            }

        }

        void setSelectedTableCard(uint8_t cardNumber, bool value) {

            this->selectedTable[cardNumber] = value;

        }

        bool getSelectedTableCard(uint8_t cardNumber) {

            return this->selectedTable[cardNumber];

        }

        void clearSelectedTableCards() {

            for (uint8_t i = 0; i < 20 ; i++) {
                this->selectedTable[i] = false;
            }

        }

        void removeHandCard(uint8_t player, uint8_t cardNumber, bool addToPlayerScore) {

            Card card = this->playerCards[player][cardNumber];

            for (uint8_t i = cardNumber; i < 3; i++) {

                this->playerCards[player][i] = this->playerCards[player][i + 1];

            }

            this->playerCards[player][2].setCardIdx(NO_CARD);

            if (addToPlayerScore) {

                this->deck[card.getCardIdx()] = (player == PLAYER_ONE ? CardStatus::PlayerOneHand : CardStatus::PlayerTwoHand);
                this->lastScore = player;

            }

        }
        
        void swapPlayers() {

            this->currentTurn = (this->currentTurn == 0 ? 1 : 0);

        }
        
        void swapDealer() {

            this->currentDeal = (this->currentDeal == 0 ? 1 : 0);
            this->currentTurn = this->currentDeal;

        }

        CardStatus getDeckCard(uint8_t cardNumber) {

            return this->deck[cardNumber];
            
        }

        uint8_t numberOfCardsInDeck() {

            return 40 - this->deckIdx;

        }

        uint8_t playerHasExactCard(uint8_t player, uint8_t deckIdx) {

            for (uint8_t i = 0; i < this->numberOfCardsInHand(PLAYER_TWO); i++) {

                Card card = this->getHandCard(PLAYER_TWO, i);

                if (card.getCardIdx() == deckIdx) {
                    return i;
                }
                
            }

            return NO_CARD;

        }

        uint8_t playerHasNumberCard(uint8_t player, uint8_t number) {

            for (uint8_t i = 0; i < this->numberOfCardsInHand(PLAYER_TWO); i++) {

                Card card = this->getHandCard(PLAYER_TWO, i);

                if (card.getNumber() == number) {
                    return i;
                }
                
            }

            return NO_CARD;

        }

        uint8_t tableValue() {

            uint8_t value = 0;

            for (uint8_t i = 0; i < this->numberOfCardsOnTable(); i++) {

                Card card = this->getTableCard(i);
                value = value + (card.getNumber() + 1);
                
            }

            return value;

        }

        uint8_t tableHasExactCard(uint8_t deckIdx) {

            for (uint8_t i = 0; i < this->numberOfCardsInHand(PLAYER_TWO); i++) {

                Card card = this->getTableCard(i);

                if (card.getCardIdx() == deckIdx) {
                    return i;
                }
                
            }

            return NO_CARD;

        }

        uint8_t tableHasNumberCard(uint8_t number) {

            for (uint8_t i = 0; i < this->numberOfCardsInHand(PLAYER_TWO); i++) {

                Card card = this->getTableCard(i);

                if (card.getNumber() == number) {
                    return i;
                }
                
            }

            return NO_CARD;

        }

        bool tableHasMatch(Card selectedCard, uint8_t matchesArray[], bool check[]) {

            uint8_t selectedNumbers[20] =   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
            uint8_t selectedDeckIdx[20] =   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
            uint8_t numberOfCardsOnTable = this->numberOfCardsOnTable();
            uint8_t score = 0;
            uint8_t matchesOverall = 0;
            uint8_t matchesThisIteration = 0;
            uint8_t cardIndex = 0;
            uint8_t bestCardIndex = 0;

            for (uint8_t i = 0; i < 20; i++) {

                matchesArray[i] = NO_CARD;

            }

            for (uint8_t x = 0; x < numberOfCardsOnTable; x++) {
                selectedNumbers[x] = this->getTableCard(x).getNumber() + 1;
                selectedDeckIdx[x] = this->getTableCard(x).getCardIdx();
            }

            for (uint8_t i = 1; i <= numberOfCardsOnTable; i++) {
                this->Combi(selectedCard, matchesThisIteration, matchesOverall, matchesArray, selectedNumbers, selectedDeckIdx, i, 0, 0, check, numberOfCardsOnTable, score, score, cardIndex, bestCardIndex);
            }

            return matchesOverall > 0;

        }

        bool tableBestPlay(uint8_t player, uint8_t &cardToPLay, uint8_t matchesArray[], bool check[]) {

            uint8_t selectedNumbers[20] =   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
            uint8_t selectedDeckIdx[20] =   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
            uint8_t numberOfCardsOnTable = this->numberOfCardsOnTable();
            uint8_t score = 0;
            uint8_t matchesOverall = 0;

            for (uint8_t i = 0; i < 20; i++) {

                matchesArray[i] = NO_CARD;
                
            }

            for (uint8_t x = 0; x < numberOfCardsOnTable; x++) {
                selectedNumbers[x] = this->getTableCard(x).getNumber() + 1;
                selectedDeckIdx[x] = this->getTableCard(x).getCardIdx();
            }

            for (uint8_t j = 0; j < this->numberOfCardsInHand(player); j++) {

                uint8_t matchesThisIteration = 0;

                Card selectedCard = this->getHandCard(player, j);
                this->Combi(selectedCard, matchesThisIteration, matchesOverall, matchesArray, selectedNumbers /* a */, selectedDeckIdx /* b */, 1, 0, 0, check, numberOfCardsOnTable, (selectedCard.getSuit() == 0 ? 5 : 0), score, j, cardToPLay);

                if (matchesThisIteration == 0) {

                    for (uint8_t i = 1; i <= numberOfCardsOnTable; i++) {

                        Card selectedCard = this->getHandCard(player, j);

                        #ifdef DEBUG
                        printf("Check %i %i >  selected card %i > ", j, i, selectedCard.getCardIdx());                    
                        #endif

                        this->Combi(selectedCard, matchesThisIteration, matchesOverall, matchesArray, selectedNumbers /* a */, selectedDeckIdx /* b */, i, 0, 0, check, numberOfCardsOnTable, (selectedCard.getSuit() == 0 ? 5 : 0), score, j, cardToPLay);

                        #ifdef DEBUG
                        printf(" allMatches %i ::", matchesOverall);
                        for (uint8_t u = 0; u < 20; u++) {
                            printf("%i ", matchesArray[u]);
                        }
                        printf("\n");
                        #endif

                    }

                }

            }

            return matchesOverall > 0;

        }


        void Combi(Card cardToMatch, uint8_t &matchesThisIteration, uint8_t &matchesOverall, uint8_t matchArray[], uint8_t a[], uint8_t b[], uint8_t reqLen, uint8_t s, uint8_t currLen, bool check[], uint8_t l, uint8_t initScore, uint8_t &score, uint8_t cardIndex, uint8_t &bestCardIndex) {

            if (currLen > reqLen) return;

            if (currLen == reqLen) {

                uint8_t cardValues = 0;
                uint8_t thisScore = 0;

                for (int i = 0; i < l; i++) {
                    if (check[i] == true) {
                        cardValues = cardValues + a[i];
                        thisScore = thisScore + (b[i] < 10 ? 10 : 0) + 1;
                    }

                }


                // Does this match the value of our card?

                if (cardValues == cardToMatch.getNumber() + 1 && thisScore + initScore > score) {

                    uint8_t j = 0;
                    score = thisScore + initScore;
                    bestCardIndex = cardIndex;

                    for (int i = 0; i < 20; i++) {
                        matchArray[j] = NO_CARD;
                    }

                    for (int i = 0; i < l; i++) {
                        if (check[i] == true) {
                            matchArray[j] = i;
                            j++;
                        }

                    }

                    matchesOverall++;
                    matchesThisIteration++;

                }

                return;

            }

            if (s == l) {
                return;
            }

            check[s] = true;
            Combi(cardToMatch, matchesThisIteration, matchesOverall, matchArray, a, b, reqLen, s + 1, currLen + 1, check, l, initScore, score, cardIndex, bestCardIndex);
            check[s] = false;
            Combi(cardToMatch, matchesThisIteration, matchesOverall, matchArray, a, b, reqLen, s + 1, currLen, check, l, initScore, score, cardIndex, bestCardIndex);

        }

        uint8_t bestDiscard(uint8_t player) {

            uint8_t numberOfCardsInHand = this->numberOfCardsInHand(player);
            uint8_t card1 = (numberOfCardsInHand > 0 ? bestDiscard_CardValue(getHandCard(player, 0)) : 0);
            uint8_t card2 = (numberOfCardsInHand > 1 ? bestDiscard_CardValue(getHandCard(player, 1)) : 0);
            uint8_t card3 = (numberOfCardsInHand > 2 ? bestDiscard_CardValue(getHandCard(player, 2)) : 0);

            if (card1 >= card2) {

                if (card1 > card3) {
                    return 0;
                }
                else {
                    return 1;
                }

            }
            else {
                return (card2 > card3 ? 1 : 2);
            }

        }

        uint8_t bestDiscard_CardValue(Card card) {

            uint8_t score = 0;

            if (card.getSuit() > 0) {

                score = score + 20;

            }

            switch (card.getNumber()) {

                case 0 ... 4:
                    score = score + 5;
                    break;

                case 7 ... 9:
                    score = score + 10;
                    break;

                case 5:
                    score = score + 3;
                    break;

                case 6:
                    score = score + 1;
                    break;

            }

            return score;

        }


};