#include <stdlib.h>
#include <stdint.h>

#include "game1.h"
#include "wii.h"
#include "ledmat.h"
#include "arm.h"


void _DisplayAction(Game1Action act) {
	LedmatClearBuffer();

	switch (act) {
		case GAME1_ACT_LEFT:
			LedmatDrawLine(CastPoint2(0,2), CastPoint2(0,5), LEDMAT_LED_RED);
			LedmatDrawLine(CastPoint2(1,2), CastPoint2(1,5), LEDMAT_LED_RED);
			break;
		case GAME1_ACT_RIGHT:
			LedmatDrawLine(CastPoint2(6,2), CastPoint2(6,5), LEDMAT_LED_RED);
			LedmatDrawLine(CastPoint2(7,2), CastPoint2(7,5), LEDMAT_LED_RED);
			break;
		case GAME1_ACT_UP:
			LedmatDrawLine(CastPoint2(2,0), CastPoint2(5,0), LEDMAT_LED_GREEN);
			LedmatDrawLine(CastPoint2(2,1), CastPoint2(5,1), LEDMAT_LED_GREEN);
			break;
		case GAME1_ACT_DOWN:
			LedmatDrawLine(CastPoint2(2,6), CastPoint2(5,6), LEDMAT_LED_GREEN);
			LedmatDrawLine(CastPoint2(2,7), CastPoint2(5,7), LEDMAT_LED_GREEN);
			break;
		case GAME1_ACT_BACK:
			LedmatDrawLine(CastPoint2(2,2), CastPoint2(5,2), LEDMAT_LED_YELLOW);
			LedmatDrawLine(CastPoint2(2,3), CastPoint2(5,3), LEDMAT_LED_YELLOW);
			break;
		case GAME1_ACT_FORWARD:
			LedmatDrawLine(CastPoint2(2,4), CastPoint2(5,4), LEDMAT_LED_YELLOW);
			LedmatDrawLine(CastPoint2(2,5), CastPoint2(5,5), LEDMAT_LED_YELLOW);
			break;
		default:
			break;
	}

	LedmatRefreshDisplay();
}

void game1(void) {
	// Default hold state
	_Game1NextState = _Game1CurrState;

	// Get and parse user input
	NunchuckData input;
	static Game1Action userAction = GAME1_ACT_NONE;
	static Game1Action lockedAction = GAME1_ACT_NONE;
	if (ReadWii(&input)) {
		// Failed read, abuse static and don't update
	}
	else {
		if (input.joy_x > 0.8) {
			userAction = GAME1_ACT_RIGHT;
		}
		else if (input.joy_x < -0.8) {
			userAction = GAME1_ACT_LEFT;
		}
		else if (input.joy_y > 0.8) {
			userAction = GAME1_ACT_UP;
		}
		else if (input.joy_y < -0.8) {
			userAction = GAME1_ACT_DOWN;
		}
		else if (input.button_c) {
			userAction = GAME1_ACT_FORWARD;
		}
		else if (input.button_z) {
			userAction = GAME1_ACT_BACK;
		}
		else {
			userAction = GAME1_ACT_NONE;
		}
	}

	switch (_Game1CurrState) {
		case GAME1_STATE_INIT:
			_Game1PrevTime = Now();
			srand(_Game1PrevTime);

			// User input needs to match the first index
			_Game1PatternDex = 0;
			_Game1DisplayDex = 0;
			// Start with a length 3 pattern
			_Game1PatternSize = 3;
			for (uint8_t i = 0; i < _Game1PatternSize; i++) {
				// 6 possible actions, NONE is 0 in the enum
				_Game1Pattern[i] = rand() % 6 + 1;
			}

			// Only leave init when the user has stopped pressing buttons
			if (userAction == GAME1_ACT_NONE) {
				_Game1NextState = GAME1_STATE_DISPLAY_PATTERN;
			}

			break;
		case GAME1_STATE_DISPLAY_PATTERN:
			// Draw the next action every second
			if (Now() - _Game1PrevTime > 1000) {
				_Game1PrevTime = Now();

				if (_Game1DisplayDex == _Game1PatternSize) {
					// Display whole pattern, display a blank
					LedmatClearBuffer();
					LedmatRefreshDisplay();

					// Then transition to wait state
					_Game1NextState = GAME1_STATE_WAIT_FOR_INPUT;
				}
				else {
					// Display next element of pattern
					_DisplayAction(_Game1Pattern[_Game1DisplayDex]);
					_Game1DisplayDex += 1;
				}
			}

			// Accept user input while displaying the pattern
			if (userAction != GAME1_ACT_NONE) {
				// Been given input, clear the screen
				LedmatClearBuffer();
				LedmatRefreshDisplay();

				// Lock in the action
				lockedAction = userAction;

				// Change to move state
				_Game1NextState = GAME1_STATE_MOVE;
			}

			break;
		case GAME1_STATE_WAIT_FOR_INPUT:
			// Check if waiting for first input
			if (_Game1PatternDex == 0) {
				// Wait for 2 seconds before showing the pattern again
				if (Now() - _Game1PrevTime > 2000) {
					_Game1PrevTime = Now();
					_Game1NextState = GAME1_STATE_DISPLAY_PATTERN;
				}
			}
			
			if (userAction != GAME1_ACT_NONE) {
				// Received input, lock it in and transfer
				lockedAction = userAction;
				_Game1NextState = GAME1_STATE_MOVE;
			}

			break;
		case GAME1_STATE_MOVE:
			// Command a move based on locked-in action
			switch (lockedAction) {
				// TODO
				default:
					break;
			}

			_Game1NextState = GAME1_STATE_CHECK;

			break;
		case GAME1_STATE_CHECK:

			// Move for a minimum amount of time
			if (Now() - _Game1PrevTime > 1000) {
				// Also require user to stop holding
				if (userAction == GAME1_ACT_NONE) {
					_Game1PrevTime = Now();

					// Check their locked answer
					if (lockedAction == _Game1Pattern[_Game1PatternDex]) {
						// Correct!
						_Game1PatternDex += 1;

						if (_Game1PatternDex >= _Game1PatternSize) {
							// User completed the pattern
							_Game1NextState = GAME1_STATE_INCREMENT_PATTERN;
						}
						else {
							// Pattern not finished yet, wait for next entry
							_Game1NextState = GAME1_STATE_WAIT_FOR_INPUT;
						}
					}
					else {
						// Wrong :(
						_Game1NextState = GAME1_STATE_DISPLAY_FAIL;
					}

					// Move the arm back to rest
					// TODO
				}
			}
			break;
		case GAME1_STATE_INCREMENT_PATTERN:
			if (_Game1PatternSize >= GAME1_MAX_PATTERN) {
				// Beat the game!!
				_Game1NextState = GAME1_STATE_DISPLAY_VICTORY;
			}
			else {
				_Game1PatternSize += 1;
				_Game1NextState = GAME1_STATE_INIT;
			}
			break;
		case GAME1_STATE_DISPLAY_FAIL:
			LedmatDrawLine(CastPoint2(1,1), CastPoint2(6,6), LEDMAT_LED_RED);
			LedmatDrawLine(CastPoint2(1,6), CastPoint2(6,1), LEDMAT_LED_RED);
			LedmatRefreshDisplay();

			if (Now() - _Game1PrevTime) {
				_Game1PrevTime = Now();
				_Game1NextState = GAME1_STATE_DISPLAY_SCORE;
			}
			break;
		case GAME1_STATE_DISPLAY_VICTORY:
			LedmatDrawPixel(CastPoint2(0,0), LEDMAT_LED_GREEN);

			if (Now() - _Game1PrevTime) {
				_Game1PrevTime = Now();
				_Game1NextState = GAME1_STATE_DISPLAY_SCORE;
			}
			break;
		case GAME1_STATE_DISPLAY_SCORE:
			LedmatClearBuffer();
			LedmatDrawNumber(_Game1PatternDex, LEDMAT_LED_YELLOW);
			LedmatRefreshDisplay();

			if (Now() - _Game1PrevTime) {
				_Game1PrevTime = Now();
				_Game1NextState = GAME1_STATE_INIT;
			}
			break;
		default:
			break;
	}

	_Game1CurrState = _Game1NextState;
}

