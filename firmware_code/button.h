#define BUTTON_DDR								DDRD
#define BUTTON_PORT_OUT							PORTD
#define BUTTON_PIN_IN							PIND
#define BUTTON_INTERRUPT						INT1
#define BUTTON_PIN								0x08
#define BUTTON_STATE_PRESSED					0
#define BUTTON_STATE_RELEASED					1

void init_button(void);
uint8_t get_button_state(void);