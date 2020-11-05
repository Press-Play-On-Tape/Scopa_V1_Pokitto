#pragma once

#include "Pokitto.h"
#include "PokittoCookie.h"
#include "Consts.h"


class GameCookie : public Pokitto::Cookie {

	public:

		uint8_t initialised;
		char initials[2][8] = { 'M', 'A', 'R', 'I', 'O', ' ', ' ', ' ', 'S', 'T', 'E', 'F', 'A', 'N', 'O', ' ' };

	public:

		void initialise() {

			this->initialised = COOKIE_INITIALISED;

			this->initials[0][0] = 'M';
			this->initials[0][1] = 'A';
			this->initials[0][2] = 'R';
			this->initials[0][3] = 'I';
			this->initials[0][4] = 'O';
			this->initials[0][5] = ' ';
			this->initials[0][6] = ' ';
			this->initials[0][7] = ' ';

			this->initials[1][0] = 'S';
			this->initials[1][1] = 'T';
			this->initials[1][2] = 'E';
			this->initials[1][3] = 'F';
			this->initials[1][4] = 'A';
			this->initials[1][5] = 'N';
			this->initials[1][6] = 'O';
			this->initials[1][7] = ' ';

			this->saveCookie();

		}

		char getPlayerChar(uint8_t player, uint8_t i) {

			return initials[player][i];

		}

		uint8_t getPlayerLength(uint8_t player) {

			uint8_t len = 0;

			for (uint8_t i = 0; i < 8; i++) {

				if (this->getPlayerChar(player, i) != 32) {
					
					len = i + 1;

				}

			}

			return len;

		}

		void setPlayerChar(uint8_t player, uint8_t i, char c) {

			initials[player][i] = c;

		}

};