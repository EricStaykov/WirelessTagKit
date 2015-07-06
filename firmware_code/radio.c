#define F_CPU									8000000UL

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "radio.h"

volatile uint8_t packet[RADIO_PACKET_BYTE_COUNT];

void init_radio_pins(void) {
	NRF_IRQ_DDR &= ~NRF_IRQ_PIN;
	// trigger an interrupt on falling edge
	EICRA |= (1 << ISC01);
	EIMSK |= (1 << NRF_IRQ_INTERRUPT);
	NRF_CSN_PORT_OUT |= NRF_CSN_PIN;
	NRF_CSN_DDR |= NRF_CSN_PIN;
	NRF_CE_PORT_OUT &= ~NRF_CE_PIN;
	NRF_CE_DDR |= NRF_CE_PIN;
}

void init_spi_master(void) {
	SPI_SS_PORT_OUT |= SPI_SS_PIN;
	SPI_DDR = (SPI_DDR & ~SPI_MISO_PIN) | SPI_SS_PIN | SPI_MOSI_PIN | SPI_SCK_PIN;
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

uint8_t spi_transmit_receive(uint8_t data) {
	SPDR = data;
	while (!(SPSR & (1 << SPIF)));
	return SPDR;
}

uint8_t radio_read_register(uint8_t reg) {
	NRF_CSN_PORT_OUT &= ~NRF_CSN_PIN;
	spi_transmit_receive(reg | RADIO_READ_REGISTER_COMMAND);
	uint8_t data = spi_transmit_receive(0);
	NRF_CSN_PORT_OUT |= NRF_CSN_PIN;
	return data;
}

void radio_write_register(uint8_t reg, uint8_t data) {
	NRF_CSN_PORT_OUT &= ~NRF_CSN_PIN;
	spi_transmit_receive(reg | RADIO_WRITE_REGISTER_COMMAND);
	spi_transmit_receive(data);
	NRF_CSN_PORT_OUT |= NRF_CSN_PIN;
}

void radio_read_payload(int byteCount, volatile uint8_t *data) {
	NRF_CSN_PORT_OUT &= ~NRF_CSN_PIN;
	spi_transmit_receive(RADIO_READ_PAYLOAD_COMMAND);
	for (int count = 0; count < byteCount; count++) {
		data[count] = spi_transmit_receive(0);
	}
	NRF_CSN_PORT_OUT |= NRF_CSN_PIN;
}

void radio_write_payload(int byteCount, volatile uint8_t *data) {
	NRF_CSN_PORT_OUT &= ~NRF_CSN_PIN;
	spi_transmit_receive(RADIO_WRITE_PAYLOAD_COMMAND);
	for (int count = 0; count < byteCount; count++) {
		spi_transmit_receive(data[count]);
	}
	NRF_CSN_PORT_OUT |= NRF_CSN_PIN;
}

void radio_flush_tx_fifo(void) {
	NRF_CSN_PORT_OUT &= ~NRF_CSN_PIN;
	spi_transmit_receive(RADIO_FLUSH_TX_FIFO_COMMAND);
	NRF_CSN_PORT_OUT |= NRF_CSN_PIN;
}

void configure_radio_send(void) {
	uint8_t configRegister = radio_read_register(RADIO_CONFIG_REGISTER);
	radio_write_register(RADIO_CONFIG_REGISTER, (configRegister & ~RADIO_CONFIG_REGISTER_PRIM_RX) | RADIO_CONFIG_REGISTER_PWR_UP);
	radio_write_register(RADIO_RX_PW_P0, RADIO_PACKET_BYTE_COUNT);
	radio_write_register(RADIO_RX_PW_P1, RADIO_PACKET_BYTE_COUNT);
}

void init_radio(void) {
	init_radio_pins();
	init_spi_master();
	configure_radio_send();
}

void radio_send_message(void) {
	radio_write_payload(RADIO_PACKET_BYTE_COUNT, packet);
	NRF_CE_PORT_OUT |= NRF_CE_PIN;
	_delay_us(15);
	NRF_CE_PORT_OUT &= ~NRF_CE_PIN;
}

// NRF IRQ pin interrupt handling
ISR(INT0_vect) {
	cli();
	// get status register
	uint8_t statusRegister = radio_read_register(RADIO_STATUS_REGISTER);
	// if status register packet sent flag set (message successfully sent and acknowledged)
	if (statusRegister & RADIO_STATUS_REGISTER_TX_DS) {
	}
	// if status register max retransmits flag set
	if (statusRegister & RADIO_STATUS_REGISTER_MAX_RT) {
		radio_flush_tx_fifo();
	}
	// clear all 3 IRQ interrupts by writing to status register
	statusRegister = radio_read_register(RADIO_STATUS_REGISTER);
	radio_write_register(RADIO_STATUS_REGISTER, statusRegister | RADIO_STATUS_REGISTER_RX_DR | RADIO_STATUS_REGISTER_TX_DS | RADIO_STATUS_REGISTER_MAX_RT);
	sei();
}