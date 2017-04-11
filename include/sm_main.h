#ifndef SM_MAIN_H
#define SM_MAIN_H

#include "wii.h"
#include "ledmat.h"
#include "game0.h"
#include "game1.h"
#include "game2.h"

enum states {S_GAME0,S_GAME1,S_GAME2,S_END};
enum events {E_NEXT, E_LAUNCH, E_NOTHING, E_END};

void UponEnter( int State);
void ActionWhileInState( int State );
void UponExit( int State);
void StateMachine(int event);
int ReadInput(void);

void sm_main(void);

#endif // SM_MAIN_H
