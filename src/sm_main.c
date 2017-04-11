#include "sm_main.h"

// Globals
int launch = 0;
uint8_t last_zbut = 0;
uint8_t last_cbut = 0;
int Current_State = S_GAME0;

void sm_main(void)
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
				game2();
			break;
        }
	}
}

int ReadInput()
{
    NunchuckData data;
    ReadWii(&data);
    uint8_t zbut = (!data.button_z) && last_zbut;
    uint8_t cbut = (!data.button_c) && last_cbut;
    last_zbut = data.button_z;
    last_cbut = data.button_c;
    if(zbut == 1){
		return E_NEXT;
	}else if(cbut == 1) {
		return E_LAUNCH;
	}else{
		return E_NOTHING;
	}
    return E_END; //
}

void StateMachine(int event)
{
    int Next_State = Current_State;
    switch ( Current_State )
    {
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
            case E_NEXT:
            	Next_State = S_GAME0;
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

    if ( event != E_END) ActionWhileInState( Current_State );
}

void UponEnter( int State)
{
    switch (State)
    {
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
    case S_GAME0:
        LedmatClearBuffer();
		LedmatDrawNumber(0, LEDMAT_LED_RED);
		LedmatRefreshDisplay();
		break;
    case S_GAME1:
        LedmatClearBuffer();
		LedmatDrawNumber(1, LEDMAT_LED_RED);
		LedmatRefreshDisplay();
		break;
    case S_GAME2:
        LedmatClearBuffer();
		LedmatDrawNumber(2, LEDMAT_LED_RED);
		LedmatRefreshDisplay();
		break;
    }
}

void UponExit( int State)
{
    switch (State)
    {
    case S_GAME0:
        LedmatClearBuffer();
        LedmatRefreshDisplay();
		break;
    case S_GAME1:
        LedmatClearBuffer();
        LedmatRefreshDisplay();
		break;
	case S_GAME2:
        LedmatClearBuffer();
        LedmatRefreshDisplay();
		break;
    }
}

