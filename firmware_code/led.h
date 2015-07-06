#define RGB_LED_DDR								DDRC
#define RGB_LED_PORT_OUT						PORTC
#define RGB_LED_R_PIN							0x01
#define RGB_LED_G_PIN							0x02
#define RGB_LED_B_PIN							0x04
#define RGB_LED_STATE_OFF						0
#define RGB_LED_STATE_R							1
#define RGB_LED_STATE_G							2
#define RGB_LED_STATE_B							3

void init_rgb_led(void);
void set_rgb_led_state(uint8_t state);