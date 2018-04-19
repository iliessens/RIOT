#include "saul.h"
#include "octa_connect.h"

static int writeRed(const void *dev, phydat_t* res) {
	(void) dev;
	octa_write_led(RED,res->val[0]);
	return 1; // 1 value processed
}

const saul_driver_t octa_saul_led_r_driver = {
	.read = saul_notsup,
	.write = writeRed,
	.type = SAUL_ACT_SWITCH,
};