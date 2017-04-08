#include <stdio.h>
#include <stdlib.h>
#include "wii.h"
#include "ledmat.h"
#include "i2c.h"
#include "servos.h"
#include "arm.h"
#include "game0.h"

enum states {S_MAIN,S_GAME0,S_GAME1,S_GAME2,S_END};
enum events {E_NEXT, E_LAUNCH, E_NOTHING, E_END}};

int Current_State = S_MAIN;

void UponEnter( int State);
void ActionWhileInState( int State );
void UponExit( int State);
void StateMachine(int event);
int ReadInput(void);
int launch = 0;

check if menu
check if game


int sm_main()
{
	if (launch == 0){
        StateMachine( ReadInput());
	}else{
		switch(Current_State)
		{
            case S_GAME0:
				game0();
			break;
            case S_GAME1:
				game1();
			break;
			case S_GAME2:
				game2()
			break;
        }
        break;
	}
}

int ReadInput()
{
    uint8_t zbut = data.fields.button_z;
    uint8_t cbut = data.fields.button_c;
    if zbut == 1{
		return E_NEXT;
        break;
	}else if{ cbut == 1
		return E_LAUNCH;
        break;
	}else{
		return E_NOTHING;
        break;
	}
    return E_END; //
}

void StateMachine(int event)
{
    int Next_State = Current_State;
    switch ( Current_State )
    {
    case S_MAIN:
        switch (event )
        {
            // A transition to the next state will occur here
            case E_NEXT:
            	Next_State = S_GAME0;
                break;
        }
        break;
    case S_GAME0:
        switch (event )
        {
            // A transition to the next state will occur here
            case E_NEXT:
            	Next_State = S_GAME1;
                break;
            case E_LAUNCH:
				launch = 1;
                break;
        }
        break;
    case S_GAME1:
        switch (event )
        {
            // A transition to the next state will occur here
            case E_NEXT:
            	Next_State = S_GAME2;
                break;
            case E_LAUNCH:
				launch = 1;
                break;
        }
        break;
    case S_GAME2:
        switch (event )
        {
            // A transition to the next state will occur here
            case E_NEXT
            	Next_State = S_MAIN;
                break;
            case E_LAUNCH:
				launch = 1;
                break;
        }
        break;
        // The program should never arrive here
		default:
        break;
    }
    if (Next_State != Current_State)
    {
        UponExit(Current_State);
        UponEnter(Next_State);
        Current_State = Next_State;
    }

    if ( event != E_MAX) ActionWhileInState( Current_State );
}

void UponEnter( int State)
{
    switch (State)
    {
    case S_MAIN:
		break;
    case S_GAME0:
		break;
    case S_GAME1:
		break;
	case S_GAME2:
		break;
    }
}

void ActionWhileInState( int State)
{
    switch (State)
    {
    case S_MAIN:
		break;
    case S_GAME0:
		LedmatDrawNumber(0, LEDMAT_LED_RED)
		LedmatRefreshDisplay()
		break;
    case S_GAME1:
		LedmatDrawNumber(1, LEDMAT_LED_RED)
		LedmatRefreshDisplay()
		break;
    case S_GAME2:
		LedmatDrawNumber(2, LEDMAT_LED_RED)
		LedmatRefreshDisplay()
		break;
    }
}

void UponExit( int State)
{
    switch (State)
    {
    case S_MAIN:
		break;
    case S_GAME0:
		break;
    case S_GAME1
		break;
	case S_GAME2:
		break;
    }
}

