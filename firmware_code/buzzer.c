#include <avr/io.h>
#include "buzzer.h"

void init_buzzer(void) {
	set_buzzer_state(BUZZER_STATE_OFF);
	BUZZER_DDR |= BUZZER_PIN;
}

void set_buzzer_state(uint8_t state) {
	if (state == BUZZER_STATE_OFF) {
		BUZZER_PORT_OUT &= ~BUZZER_PIN;
	} else if (state == BUZZER_STATE_ON) {
		BUZZER_PORT_OUT |= BUZZER_PIN;
	}
}