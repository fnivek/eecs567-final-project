#ifndef SERVOS_H
#define SERVOS_H

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>

#include "helpers.h"

// Structure that represents a motor
typedef struct {
	Pin pwm_pin;
	uint32_t timer;
	enum tim_oc_id	output_compare_channel;
} Servo;

typedef enum ServoIndex {
	kBase,
	kShoulder,
	kElbow,
	kWrist,

	kNumServos
} ServoIndex;

static const Servo servos[] = {
	//pwm,			timmer,		output_compare_channel
	{{GPIOA, GPIO1}, TIM2, 		TIM_OC2},		// kBase
	{{GPIOA, GPIO2}, TIM2, 		TIM_OC3},		// kShoulder
	{{GPIOA, GPIO3}, TIM2, 		TIM_OC4},		// kShoulder
	{{GPIOA, GPIO5}, TIM2, 		TIM_OC1}		// kShoulder
};

// Timer info http://www.st.com/content/ccc/resource/technical/document/reference_manual/3d/6d/5a/66/b4/99/40/d4/DM00031020.pdf/files/DM00031020.pdf/jcr:content/translations/en.DM00031020.pdf pg 612
// Max number of pwm ticks
//		Ticks_per_period = period * clk_freq / (2 * prescale)
//				840,000  = 0.01   *  168 Mhz / (2 *     1   )
static const uint32_t kTicksPerPeriod = 840000;

// Pulse width values
//		ticks_per_pulse_width (tPW) = ticks_per_period * pulse_width / period
//					63,000          =  840000 * 1.5ms / 20ms
static const uint32_t kZeroPW = 63000;
//					42,000			=  840000 * 1ms / 20ms 
static const uint32_t kMinPW = 42000;
//					84,000			=  840000 * 2ms / 20ms
static const uint32_t kMaxPW = 84000;

void SetupServos(void);
void ServosSetPWM(ServoIndex index, uint32_t pwm);
void ServosZero(void);

#endif