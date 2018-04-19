
#include "octa_connect.h"
#include "periph/gpio.h"

typedef struct {
	gpio_t red;
	gpio_t green;
	gpio_t blue;
} octa_leds_t;

octa_leds_t octa_leds;

void init_leds(void) {
	octa_leds.green = GPIO_PIN(PORT_B,0);
	gpio_init(octa_leds.green,GPIO_OUT);
	gpio_write(octa_leds.green, 1); // default to off
	
	octa_leds.red = GPIO_PIN(PORT_D,14);
	gpio_init(octa_leds.red,GPIO_OUT);
	gpio_write(octa_leds.red, 1); // default to off
	
	octa_leds.blue = GPIO_PIN(PORT_D,15);
	gpio_init(octa_leds.blue,GPIO_OUT);
	gpio_write(octa_leds.blue, 1); // default to off
}

int octa_connect_init(void) {
	
	init_leds();
	
	return 0;
}

void octa_write_led(octa_led_color_t color, int state) {
	gpio_t* pin = NULL;
	
	switch(color) {
		case(RED): pin = &octa_leds.red;
		break;
		case(GREEN): pin = &octa_leds.green;
		break;
		case(BLUE): pin = &octa_leds.blue;
		break;
	}
	
	if(pin == NULL) return;
	puts("Writing LED");
	
	gpio_write(*pin, (state == 1) ? 0 : 1);
}
