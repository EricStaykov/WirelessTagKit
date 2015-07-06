// nrf interrupt pin
#define NRF_IRQ_DDR								DDRD
#define NRF_IRQ_PIN								0x04
#define NRF_IRQ_INTERRUPT						INT0
// nrf spi slave select pin
#define NRF_CSN_DDR								DDRD
#define NRF_CSN_PORT_OUT						PORTD
#define NRF_CSN_PIN								0x02
// nrf chip enable pin
#define NRF_CE_DDR								DDRD
#define NRF_CE_PORT_OUT							PORTD
#define NRF_CE_PIN								0x01
// spi specific pins
#define SPI_DDR									DDRB
#define SPI_SS_PORT_OUT							PORTB
#define SPI_SS_PIN								0x04
#define SPI_MOSI_PIN							0x08
#define SPI_MISO_PIN							0x10
#define SPI_SCK_PIN								0x20
// nrf commands
#define RADIO_READ_REGISTER_COMMAND				0x00
#define RADIO_WRITE_REGISTER_COMMAND			0x20
#define RADIO_READ_PAYLOAD_COMMAND				0x61
#define RADIO_WRITE_PAYLOAD_COMMAND				0xA0
#define RADIO_FLUSH_TX_FIFO_COMMAND				0xE1
// nrf registers and their bits
#define RADIO_CONFIG_REGISTER					0x00
#define RADIO_CONFIG_REGISTER_PRIM_RX			0x01
#define RADIO_CONFIG_REGISTER_PWR_UP			0x02
#define RADIO_STATUS_REGISTER					0x07
#define RADIO_STATUS_REGISTER_RX_DR				0x40
#define RADIO_STATUS_REGISTER_TX_DS				0x20
#define RADIO_STATUS_REGISTER_MAX_RT			0x10
#define RADIO_FIFO_STATUS_REGISTER				0x17
#define RADIO_FIFO_STATUS_REGISTER_RX_EMPTY		0x01
#define RADIO_RX_PW_P0							0x11
#define RADIO_RX_PW_P1							0x12
// bytes in a single packet
#define RADIO_PACKET_BYTE_COUNT					32

extern volatile uint8_t packet[];

void init_radio_pins(void);
void init_spi_master(void);
uint8_t spi_transmit_receive(uint8_t data);
uint8_t radio_read_register(uint8_t reg);
void radio_write_register(uint8_t reg, uint8_t data);
void radio_read_payload(int byteCount, volatile uint8_t *data);
void radio_write_payload(int byteCount, volatile uint8_t *data);
void radio_flush_tx_fifo(void);
void configure_radio_send(void);
void init_radio(void);
void radio_send_message(void);
ISR(INT0_vect);