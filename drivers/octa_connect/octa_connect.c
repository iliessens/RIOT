
#include "octa_connect.h"
#include "octa_gpio.h"

int octa_connect_init(void) {
	
	init_leds();
	init_btn();
	
	return 0;
}