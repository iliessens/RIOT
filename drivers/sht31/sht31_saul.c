#include <string.h>
#include "saul.h"
#include "sht31.h"

#include <stdio.h>

static int read_temp(const void * dev, phydat_t * res) {
	res->unit = UNIT_TEMP_C;
	res->scale = -2;
	
	sht31_read_temp(dev, &(res->val[0]));
	
	return 1;
}

const saul_driver_t sht31_saul_temp_driver = {
    .read = read_temp,
    .write = saul_notsup,
    .type = SAUL_SENSE_TEMP,
};