#ifndef DEBUG_LEDS_H
#define DEBUG_LEDS_H

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#include "systick_handler.h"

typedef uint16_t DebugLed;
static const DebugLed kDebugGreenLed =		GPIO12;
static const DebugLed kDebugOrangeLed =		GPIO13;
static const DebugLed kDebugRedLed =		GPIO14;
static const DebugLed kDebugBlueLed =		GPIO15;
static const DebugLed kDebugAllLeds =		GPIO12 | GPIO13 | GPIO14 | GPIO15;

void SetupDebugLeds(void);
void SetHeartbeatLed(DebugLed color);
void Beat(void);
void SetDebugLed(DebugLed color);
void ClearDebugLed(DebugLed color);
void ToggleDebugLed(DebugLed color);

#endif