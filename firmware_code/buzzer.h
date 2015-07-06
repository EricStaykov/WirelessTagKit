#define BUZZER_DDR								DDRB
#define BUZZER_PORT_OUT							PORTB
#define BUZZER_PIN								0x80
#define BUZZER_STATE_OFF						0
#define BUZZER_STATE_ON							1

void init_buzzer(void);
void set_buzzer_state(uint8_t state);