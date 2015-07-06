#define F_CPU									8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "temperature.h"

void tx_mode(void) {
	DS18B20_DATA_DDR |= DS18B20_DATA_PIN;
}

void rx_mode(void) {
	DS18B20_DATA_DDR &= (~DS18B20_DATA_PIN);
}

void tx_low(void) {
	tx_mode();
	DS18B20_DATA_PORT_OUT &= (~DS18B20_DATA_PIN);
}

uint8_t rx_read_bit(void) {
	rx_mode();
	return !!(DS18B20_DATA_PIN_IN & DS18B20_DATA_PIN);
}

void init_one_wire(void) {
	rx_mode();
	_delay_ms(1);
}

void init_sequence(void) {
	tx_low();
	_delay_ms(1);
	rx_mode();
	_delay_ms(1);
}

void issue_command(uint8_t command) {
	for (int bit = 0; bit < 8; bit++) {
		tx_low();
		if ((command >> bit) & 1) {
			_delay_us(1);
		} else {
			_delay_us(70);
		}
		rx_mode();
		_delay_ms(1);
	}	
}

int16_t get_temp_sequence(void) {
	int16_t temp = 0;
	int bit;
	for (int position = 0; position < 16; position++) {
		tx_low();
		_delay_us(2);
		rx_mode();
		_delay_us(1);
		bit = rx_read_bit();
		temp |= (bit << position);
		_delay_ms(1);
	}
	init_sequence();
	return temp;
}

void gen_temp(void) {
	init_sequence();
	issue_command(DS18B20_COMMAND_SKIP_ROM);
	issue_command(DS18B20_COMMAND_GEN_TEMP);
	_delay_ms(800);
}

int16_t get_temp(void) {
	init_sequence();
	issue_command(DS18B20_COMMAND_SKIP_ROM);
	issue_command(DS18B20_COMMAND_GET_TEMP);
	return get_temp_sequence();
}

float convert_temp(int16_t rawTemp) {
	return ((float)rawTemp) / 16.0;
}