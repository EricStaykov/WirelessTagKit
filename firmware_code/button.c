#include <avr/io.h>
#include "button.h"

void init_button(void) {
	BUTTON_DDR &= ~BUTTON_PIN;
	BUTTON_PORT_OUT |= BUTTON_PIN;
	// trigger an interrupt on any state change
	EICRA |= (1 << ISC10);
	EIMSK |= (1 << BUTTON_INTERRUPT);
}

uint8_t get_button_state(void) {
	return (BUTTON_PIN_IN & BUTTON_PIN) ? BUTTON_STATE_RELEASED : BUTTON_STATE_PRESSED;
}