#define F_CPU									8000000UL

#include <avr/interrupt.h>
#include <avr/io.h>
#include <ctype.h>
#include <inttypes.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include <util/twi.h>
#include "button.h"
#include "buzzer.h"
#include "led.h"
#include "radio.h"
#include "temperature.h"

void init_hardware(void) {
	init_rgb_led();
	init_buzzer();
	init_button();
	init_one_wire();
	init_radio();
	sei();
}

void update_temperature(volatile uint8_t *packet) {
	gen_temp();
	int16_t rawTemp = get_temp();
	float convertedTemp = convert_temp(rawTemp);
	for (uint8_t loop = 0; loop < 32; loop++) {
		packet[loop] = '/';
	}
	char sign = (convertedTemp < 0) ? '-' : '+';
	packet[0] = sign;
	convertedTemp = (convertedTemp < 0) ? (convertedTemp * -1) : convertedTemp;
	uint8_t steps = 7;
	uint8_t digits[steps];
	float current = 10000;
	for (uint8_t loop = 0; loop < steps; loop++) {
		digits[loop] = (uint8_t)(fmod(convertedTemp, current) / (current / 10)) + '0';
		current /= 10;
	}
	packet[1] = digits[0];
	packet[2] = digits[1];
	packet[3] = digits[2];
	packet[4] = digits[3];
	packet[5] = '.';
	packet[6] = digits[4];
	packet[7] = digits[5];
	packet[8] = digits[6];
}

int main(void) {
	init_hardware();
	while (1) {
		update_temperature(packet);
		radio_send_message();
		set_rgb_led_state(RGB_LED_STATE_R);
		_delay_ms(250);
		set_rgb_led_state(RGB_LED_STATE_G);
		_delay_ms(250);
		set_rgb_led_state(RGB_LED_STATE_B);
		_delay_ms(250);
		set_rgb_led_state(RGB_LED_STATE_OFF);
		_delay_ms(250);
	}
	return 1;
}

// button state change interrupt
ISR(INT1_vect) {
	cli();
	if (get_button_state() == BUTTON_STATE_PRESSED) {
		set_buzzer_state(BUZZER_STATE_ON);
	} else {
		set_buzzer_state(BUZZER_STATE_OFF);
	}
	sei();
}