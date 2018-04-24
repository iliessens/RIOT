#include "periph/gpio.h"
#include "board.h"
#include "octa_gpio.h"

typedef struct {
	gpio_t red;
	gpio_t green;
	gpio_t blue;
	gpio_t btn1;
	gpio_t btn2;
} octa_gpio_t;

octa_gpio_t octa_gpio;

void init_leds(void) {
	octa_gpio.green = GPIO_PIN(PORT_B,0);
	gpio_init(octa_gpio.green,GPIO_OUT);
	gpio_write(octa_gpio.green, 1); // default to off
	
	octa_gpio.red = GPIO_PIN(PORT_D,14);
	gpio_init(octa_gpio.red,GPIO_OUT);
	gpio_write(octa_gpio.red, 1); // default to off
	
	octa_gpio.blue = GPIO_PIN(PORT_D,15);
	gpio_init(octa_gpio.blue,GPIO_OUT);
	gpio_write(octa_gpio.blue, 1); // default to off
}

void init_btn(void) {
	octa_gpio.btn1 = GPIO_PIN(PORT_G,0);
	gpio_init(octa_gpio.btn1,GPIO_IN);
	
	octa_gpio.btn2 = GPIO_PIN(PORT_G,1);
	gpio_init(octa_gpio.btn2,GPIO_IN);
}

gpio_t* color_to_pin(octa_led_color_t color) {
	switch(color) {
		case(RED): 		return &octa_gpio.red;
		case(GREEN): 	return &octa_gpio.green;
		case(BLUE): 	return &octa_gpio.blue;
		default: 		return NULL;
	}
}

void octa_write_led(octa_led_color_t color, int state) {
	gpio_t* pin = color_to_pin(color);
	
	if(pin == NULL) return;
	
	gpio_write(*pin, (state == 1) ? 0 : 1);
}

int octa_read_led(octa_led_color_t color) {
	gpio_t* pin = color_to_pin(color);
	
	if(pin == NULL) {
		puts("Octa: Invalid LED color");
		return 0;
	}
	
	int result = gpio_read(*pin);
	if(result > 0) return 0;
	else return 1;
}

int octa_read_button(int button) {
	gpio_t* pin = NULL;
	
	switch(button) {
		case(1): pin = &octa_gpio.btn1;
		break;
		
		case(2): pin = &octa_gpio.btn2;
		break;
	}
	
	if(pin==NULL) puts("Invalid button. [1 or 2]");
	
	int result = gpio_read(*pin);
	
	//result > 0 unpressed
	
	// return inverted value
	if(result == 0) return 1;
	else return 0;
}
