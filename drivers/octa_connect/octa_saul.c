#include "saul.h"
#include "octa_connect.h"

static int writeLED(const void *dev, phydat_t* res) {
	const octa_led_t* ledDev = (const octa_led_t *) dev;
	octa_write_led(ledDev->color, res->val[0]);
	return 1; // 1 value processed
}

// saul driver definitions

const saul_driver_t octa_saul_led_driver = {
	.read = saul_notsup,
	.write = writeLED,
	.type = SAUL_ACT_SWITCH,
};