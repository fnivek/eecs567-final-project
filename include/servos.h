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

enum ServoIndex {
	kBase,
	kShoulder,
	kElbow,
	kWrist,

	kNumServos
};

static const Servo servos[] = {
	//pwm,			timmer,		output_compare_channel
	{{GPIOA, GPIO1}, TIM2, 		TIM_OC2},		// kBase
	{{GPIOA, GPIO2}, TIM2, 		TIM_OC3},		// kShoulder
	{{GPIOA, GPIO3}, TIM2, 		TIM_OC4},		// kShoulder
	{{GPIOA, GPIO5}, TIM2, 		TIM_OC1}		// kShoulder
};
// Max number of pwm ticks 
//		168 MHZ / (2 * 20 KHz) 
static const uint32_t kMaxPwmTicks = 4200;

void SetupServos(void);
void ServosSetPWM(enum ServoIndex index, uint32_t pwm);
void ServosZero(void);

#endif