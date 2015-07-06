#include <avr/io.h>
#include "led.h"

void init_rgb_led(void) {
	set_rgb_led_state(RGB_LED_STATE_OFF);
	RGB_LED_DDR |= (RGB_LED_R_PIN | RGB_LED_G_PIN | RGB_LED_B_PIN);
}

void set_rgb_led_state(uint8_t state) {
	if (state == RGB_LED_STATE_OFF) {
		RGB_LED_PORT_OUT |= (RGB_LED_R_PIN | RGB_LED_G_PIN | RGB_LED_B_PIN);
	} else if (state == RGB_LED_STATE_R) {
		RGB_LED_PORT_OUT = (RGB_LED_PORT_OUT & ~RGB_LED_R_PIN) | RGB_LED_G_PIN | RGB_LED_B_PIN;
	} else if (state == RGB_LED_STATE_G) {
		RGB_LED_PORT_OUT = (RGB_LED_PORT_OUT & ~RGB_LED_G_PIN) | RGB_LED_B_PIN | RGB_LED_R_PIN;
	} else if (state == RGB_LED_STATE_B) {
		RGB_LED_PORT_OUT = (RGB_LED_PORT_OUT & ~RGB_LED_B_PIN) | RGB_LED_R_PIN | RGB_LED_G_PIN;
	}
}