#include <string.h> // for memset

#include "saul.h"
#include "octa_connect.h"

static int writeLED(const void *dev, phydat_t* res) {
	const octa_led_t* ledDev = (const octa_led_t *) dev;
	octa_write_led(ledDev->color, res->val[0]);
	return 1; // 1 value processed
}

static int readLED(const void * dev, phydat_t* state) {
	state->scale = 0;
	state->unit = UNIT_BOOL;
	
	const octa_led_t* ledDev = (const octa_led_t *) dev;
	state->val[0] = octa_read_led(ledDev->color);
	
	memset(&(state->val[1]), 0, 2 * sizeof(uint16_t)); // set two other fields to 0
	return 1;
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
	.type = SAUL_ACT_SWITCH,
};

const saul_driver_t octa_saul_btn_driver = {
	.read = readButton,
	.write = saul_notsup,
	.type = SAUL_SENSE_BTN,
};