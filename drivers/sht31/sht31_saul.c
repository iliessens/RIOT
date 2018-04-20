#include <string.h>
#include "saul.h"

#include <stdio.h>

static int read_temp(const void * dev, phydat_t * res) {
	(void) dev;
	
	res->unit = UNIT_TEMP_C;
	
	puts("Not implented :)");
	
	return 1;
}

const saul_driver_t sht31_saul_temp_driver = {
    .read = read_temp,
    .write = saul_notsup,
    .type = SAUL_SENSE_TEMP,
};