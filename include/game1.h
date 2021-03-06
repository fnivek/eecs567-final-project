#ifndef GAME1_H
#define GAME1_H

#define GAME1_MAX_PATTERN		9

typedef enum {
	GAME1_ACT_NONE,
	GAME1_ACT_LEFT,
	GAME1_ACT_RIGHT,
	GAME1_ACT_UP,
	GAME1_ACT_DOWN,
	GAME1_ACT_BACK,
	GAME1_ACT_FORWARD
} Game1Action;

typedef enum {
	GAME1_STATE_INIT,
	GAME1_STATE_DISPLAY_PATTERN,
	GAME1_STATE_WAIT_FOR_INPUT,
	GAME1_STATE_MOVE,
	GAME1_STATE_CHECK,
	GAME1_STATE_INCREMENT_PATTERN,
	GAME1_STATE_DISPLAY_FAIL,
	GAME1_STATE_DISPLAY_VICTORY,
	GAME1_STATE_DISPLAY_SCORE
} Game1State;

Game1State _Game1CurrState;
Game1State _Game1NextState;

Game1Action _Game1Pattern[GAME1_MAX_PATTERN];
uint8_t _Game1PatternSize;
uint8_t _Game1PatternDex;

uint32_t _Game1PrevTime;
uint8_t _Game1DisplayDex;

void game1(void);



#endif // GAME1_H
