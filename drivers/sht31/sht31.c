// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// SHT31
// This code is designed to work with the SHT31_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Humidity?sku=SHT31_I2CS#tabs-0-product_tabset-2

#include "sht31.h"

#include <string.h>
#include "xtimer.h"

#define ENABLE_DEBUG (1)
#include "debug.h"

#define I2C_SPEED 	I2C_SPEED_FAST
#define BUS 		(dev->p.i2c)
#define ADDR		(dev->p.addr)


i2c_t dev = I2C_DEV(0);
uint8_t address = 0x44;

int sht31_init(sht31_t* dev, const sht31_params_t* params)
{
	DEBUG("Copying parameters...\n");
	
	memcpy(&(dev->p), params, sizeof(sht31_params_t)); // save params
	int status = SHT31_OK;
	
	if(i2c_acquire(BUS)) status = SHT31_NOI2C;
	if(i2c_init_master(BUS, I2C_SPEED)) status = SHT31_NOI2C;;
	
	// Send high repeatability measurement command
	// Command msb, command lsb(0x24, 0x00)
	
	if(status == SHT31_OK) {
		DEBUG("Bus aqcuired and init OK\n");
		
		char config[2] = {0};
		
		config[0] = 0x21;
		config[1] = 0x30;

		if( i2c_write_bytes(BUS, address, &config[0], 2) != 2) status = SHT31_ECONFIG;
		
		DEBUG("Config written\n");
	}
	
	i2c_release(BUS);
	
	DEBUG("SHT31 init done, status %d\n", status);
	
	return status;
}

void sht31_read(sht31_t* dev) {
	// Get I2C device, SHT31 I2C address is 0x44(68)
	
	i2c_acquire(BUS);

	// Read 6 bytes of data
	// temp msb, temp lsb, temp CRC, humidity msb, humidity lsb, humidity CRC
	char data[6] = {0};
	
	//wait until we get a valid measurement (num of bytes > 0)
	while(i2c_read_bytes(BUS, ADDR, &data[0], 6) == 0) ;
	
	// Convert the data
//	double cTemp = (((data[0] * 256) + data[1]) * 175.0) / 65535.0  - 45.0;
	uint16_t cTemp = ((data[0] * 256) + data[1]) * 267  - 45.0;
	double humidity = (((data[3] * 256) + data[4])) * 100.0 / 65535.0;

	// Output data to screen
	printf("Temperature in Celsius : %d C \n", cTemp);
	printf("Relative Humidity is : %.2f RH \n", humidity);
	
	i2c_release(BUS);
}

// value nog te scalen met 10E⁻5
void sht31_read_temp(const sht31_t* dev, int16_t* result) {
	// Get I2C device, SHT31 I2C address is 0x44(68)
	
	i2c_acquire(BUS);

	// Read 6 bytes of data
	// temp msb, temp lsb, temp CRC, humidity msb, humidity lsb, humidity CRC
	char data[6] = {0};
	
	//wait until we get a valid measurement (num of bytes > 0)
	// backoff for a short period
	while(i2c_read_bytes(BUS, ADDR, &data[0], 6) == 0) xtimer_usleep(20);
	
	// TODO remove floating point arithmetic
	double cTemp = (((data[0] * 256) + data[1]) * 175.0) / 65535.0  - 45.0;
	*result = (int16_t) (cTemp * 100);
	
	i2c_release(BUS);
}
