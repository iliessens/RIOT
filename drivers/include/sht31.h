
#ifndef SHT31_H
#define SHT31_H

#include "periph/i2c.h"

enum {
	SHT31_OK = 0,
	SHT31_NOI2C,
	SHT31_ECONFIG
};

typedef struct {
	char addr; 
	i2c_t i2c;
} sht31_params_t;

typedef struct {
	sht31_params_t p;
} sht31_t;

int sht31_init(sht31_t* dev, const sht31_params_t* params);
void sht31_read(sht31_t* dev);

#endif
