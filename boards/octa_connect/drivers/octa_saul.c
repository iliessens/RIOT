#include <string.h> // for memset

#include "saul.h"
#include "board.h"
#include "octa_gpio.h"
#include <stdio.h>

static int writeLED(const void *dev, phydat_t* res) {
	(void) dev;
	
	for(int i = 0; i < 3; i++) {
		int16_t val = res->val[i];
		// prevent bigger values from causing problems
		if(val > 0) val = 1;
		else val= 0;
		
		octa_write_led(i, val);
	}
	
	return 3;
}

static int readLED(const void * dev, phydat_t* state) {
	(void) dev;
	
	state->scale = 0;
	state->unit = UNIT_BOOL;
	
	for(int i = 0; i < 3; i++) {
		state->val[i] = octa_read_led(i);
	}
	
	return 3;
}

static int readButton(const void *dev, phydat_t* state) {
	state->scale = 0;
	state->unit = UNIT_BOOL;
	
	const int* btn_num = ((const int*) dev);
	
	state->val[0] = octa_read_button(*btn_num);
	
	memset(&(state->val[1]), 0, 2 * sizeof(uint16_t)); // set two other fields to 0
	return 1;
}

// saul driver definitions

const saul_driver_t octa_saul_led_driver = {
	.read = readLED,
	.write = writeLED,
	.type = SAUL_ACT_LED_RGB,
};

const saul_driver_t octa_saul_btn_driver = {
	.read = readButton,
	.write = saul_notsup,
	.type = SAUL_SENSE_BTN,
};
