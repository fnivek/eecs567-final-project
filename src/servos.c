#include "servos.h"

void SetupServos(void) {
	// Enable peripherials clocks
	rcc_periph_clock_enable(RCC_TIM2);
	rcc_periph_clock_enable(RCC_GPIOA);

	// Timer
	// Step 1 reset peripherial:
	timer_reset(TIM2);
	// Step 2 set mode
	timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_CENTER_1,
               TIM_CR1_DIR_UP);
	// Step 3 set output compare to PWM for all channels
	timer_set_oc_mode(TIM2, TIM_OC1, TIM_OCM_PWM1);
	timer_set_oc_mode(TIM2, TIM_OC2, TIM_OCM_PWM1);
	timer_set_oc_mode(TIM2, TIM_OC3, TIM_OCM_PWM1);
	timer_set_oc_mode(TIM2, TIM_OC4, TIM_OCM_PWM1);
	// Step 4 enable output compare for all channels
	timer_enable_oc_output(TIM2, TIM_OC1);
	timer_enable_oc_output(TIM2, TIM_OC2);
	timer_enable_oc_output(TIM2, TIM_OC3);
	timer_enable_oc_output(TIM2, TIM_OC4);
	// Step 5 set the compare value
	timer_set_oc_value(TIM2, TIM_OC1, 0);
	timer_set_oc_value(TIM2, TIM_OC2, 0);
	timer_set_oc_value(TIM2, TIM_OC3, 0);
	timer_set_oc_value(TIM2, TIM_OC4, 0);
	// Step 6 set the period
	timer_set_period(TIM2, kTicksPerPeriod);
	// Step 7 enable break
	timer_enable_break_main_output(TIM2);
	// Step 8 enable done last

	// GPIO
	int current_servo_index;
	for(current_servo_index = 0; current_servo_index < kNumServos; ++current_servo_index)
	{
		const Servo* servo = &servos[current_servo_index];

		// Step 1 set output options (push pull) output, no pull up or pull down
		gpio_set_output_options(servo->pwm_pin.port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, servo->pwm_pin.number);

		// Step 2 setup mode
		gpio_mode_setup(servo->pwm_pin.port, GPIO_MODE_AF, GPIO_PUPD_NONE, servo->pwm_pin.number);
		gpio_set_af(servo->pwm_pin.port, GPIO_AF1, servo->pwm_pin.number);
	}

	// Set servos to there zero position
	ServosZero();

	// Timer
	// Step 8 enable the timer
	timer_enable_counter(TIM2);

}

void ServosSetPWM(ServoIndex index, uint32_t pwm) {
	const Servo* servo = &servos[index];

	if (pwm > kMaxPW) {
		pwm = kMaxPW;
	} else if(pwm < kMinPW) {
		pwm = kMinPW;
	}

	timer_set_oc_value(servo->timer, servo->output_compare_channel, pwm);
}

void ServosZero(void) {
	uint8_t index;
	for(index = 0; index < kNumServos; ++index) {
		ServosSetPWM(index, kZeroPW);
	}
}