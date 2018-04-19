#include "saul.h"
#include "octa_connect.h"

static int writeRed(const void *dev, phydat_t* res) {
	(void) dev;
	octa_write_led(RED,res->val[0]);
	return 1; // 1 value processed
}

static int writeGreen(const void *dev, phydat_t* res) {
	(void) dev;
	octa_write_led(GREEN,res->val[0]);
	return 1; // 1 value processed
}
static int writeBlue(const void *dev, phydat_t* res) {
	(void) dev;
	octa_write_led(BLUE,res->val[0]);
	return 1; // 1 value processed
}

// saul driver definitions

const saul_driver_t octa_saul_led_r_driver = {
	.read = saul_notsup,
	.write = writeRed,
	.type = SAUL_ACT_SWITCH,
};

const saul_driver_t octa_saul_led_g_driver = {
	.read = saul_notsup,
	.write = writeGreen,
	.type = SAUL_ACT_SWITCH,
};

const saul_driver_t octa_saul_led_b_driver = {
	.read = saul_notsup,
	.write = writeBlue,
	.type = SAUL_ACT_SWITCH,
};