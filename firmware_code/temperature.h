#define DS18B20_DATA_DDR						DDRA
#define DS18B20_DATA_PORT_OUT					PORTA
#define DS18B20_DATA_PIN_IN						PINA
#define DS18B20_DATA_PIN						0x08
#define DS18B20_COMMAND_SKIP_ROM				0xCC
#define DS18B20_COMMAND_GEN_TEMP				0x44
#define DS18B20_COMMAND_GET_TEMP				0xBE

void tx_mode(void);
void rx_mode(void);
void tx_low(void);
uint8_t rx_read_bit(void);
void init_one_wire(void);
void init_sequence(void);
void issue_command(uint8_t command);
int16_t get_temp_sequence(void);
void gen_temp(void);
int16_t get_temp(void);
float convert_temp(int16_t rawTemp);