#ifndef SYSTICK_HANDLER_H
#define SYSTICK_HANDLER_H

#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/nvic.h>

// constants
#define kMaxCallbacks 10

// typedef for void callbacks
typedef void (*VoidCB)(void);

// Globals
volatile uint32_t system_millis;

// Types
typedef struct SystickCallback
{
	// Callback to a function which performs the desired operation
	VoidCB callback;

	// The period in milliseconds at which to call the callback
	uint32_t period;

	// Call count (the callback should be called this many times)
	uint8_t call_count;
} SystickCallback;

typedef struct SystickCallbackList
{
	uint8_t index;
	SystickCallback callback_list[kMaxCallbacks];

} SystickCallbackList;


// Functions
void sys_tick_handler(void);

void SetupSystick(void);

int AddSystickCallback(VoidCB callback, uint32_t period);
int RemoveSystickCallback(VoidCB callback);

void DoSystickCallbacks(void);

// Return mills since start
uint32_t Now(void);

#endif