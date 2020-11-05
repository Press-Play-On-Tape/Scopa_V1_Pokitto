#include "Pokitto.h"
#include "../utils/Consts.h"
#include "../utils/Utils.h"

struct Card {

    private:

        uint8_t suit = 0;
        uint8_t number = 0;
        uint8_t cardIdx = 0;

    public:

        int8_t getSuit()                        { return this->suit; }
        int8_t getNumber()                      { return this->number; }
        int8_t getCardIdx()                     { return this->cardIdx; }

        void setSuit(int8_t val)                { this->suit = val; }
        void setNumber(int8_t val)              { this->number = val; }

    public:

        void setCardIdx(uint8_t cardIdx) {

            this->cardIdx = cardIdx;
            this->suit = cardIdx / 10;
            this->number = cardIdx % 10;

        }

        int cmp (const void *ptr1 , const void *ptr2) {

            Card *c1 = *(Card**)ptr1;
            Card *c2 = *(Card**)ptr2;

            return c1->getNumber() < c2->getNumber();
        }

};