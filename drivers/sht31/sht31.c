// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// SHT31
// This code is designed to work with the SHT31_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Humidity?sku=SHT31_I2CS#tabs-0-product_tabset-2

#include "sht31.h"

#include <stdio.h>
#include <stdlib.h>
#include "periph/i2c.h"
#include "xtimer.h"

#define I2C_SPEED I2C_SPEED_FAST

i2c_t dev = I2C_DEV(0);
uint8_t address = 0x44;

void sht31_init(void) 
{
	i2c_acquire(dev);
	
	i2c_init_master(dev, I2C_SPEED);
	
	// Send high repeatability measurement command
	// Command msb, command lsb(0x2C, 0x06)
	char config[2] = {0};
	//config[0] = 0x2C;
	//config[1] = 0x06;
	
	config[0] = 0x24;
	config[1] = 0x00;

	i2c_write_bytes(dev, address, &config[0], 2);
	
	i2c_release(dev);
	
	//xtimer_sleep(1);
}

void sht31_read(void) {
	// Get I2C device, SHT31 I2C address is 0x44(68)
	
	i2c_acquire(dev);
	
	//sleep(1);

	// Read 6 bytes of data
	// temp msb, temp lsb, temp CRC, humidity msb, humidity lsb, humidity CRC
	char data[6] = {0};
	
	//wait until we get a valid measurement (num of bytes > 0)
	while(i2c_read_bytes(dev, address, &data[0], 6) == 0) ;
	
	// Convert the data
	double cTemp = (((data[0] * 256) + data[1]) * 175.0) / 65535.0  - 45.0;
	double humidity = (((data[3] * 256) + data[4])) * 100.0 / 65535.0;

	// Output data to screen
	printf("Temperature in Celsius : %.2f C \n", cTemp);
	printf("Relative Humidity is : %.2f RH \n", humidity);
	
	i2c_release(dev);
	}
