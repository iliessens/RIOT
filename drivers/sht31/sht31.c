/* 
 * Copyright 2018 University of Antwerp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string.h>
#include "sht31.h"

#include "xtimer.h"

#define ENABLE_DEBUG (0)
#include "debug.h"

#define SHT31_MSB_1MPS 0x21
#define SHT31_LSB_HIGH_REPEATABILITY 0x30
#define SHT31_CONFIG_LENGTH 2
#define SHT31_MEASUREMENT_BACKOFF 20

#define I2C_SPEED 	I2C_SPEED_FAST
#define BUS 		(dev->p.i2c)
#define ADDR		(dev->p.addr)

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
		
		uint8_t config[SHT31_CONFIG_LENGTH] = {0};
		
		config[0] = SHT31_MSB_1MPS;
		config[1] = SHT31_LSB_HIGH_REPEATABILITY;

		if( i2c_write_bytes(BUS, ADDR, &config[0], SHT31_CONFIG_LENGTH) != SHT31_CONFIG_LENGTH) {
			status = SHT31_ECONFIG;
		}
		
		if(status == SHT31_OK) DEBUG("Config written\n");
	}
	
	i2c_release(BUS);
	
	DEBUG("SHT31 init done, status %d\n", status);
	
	return status;
}

enum { TEMP, HUM };

// TODO maybe use CRC
static void _read_sensor(const sht31_t* dev, int16_t* result, int type) {
	// Get I2C device, SHT31 I2C address is 0x44(68)
	
	i2c_acquire(BUS);

	// Read 6 bytes of data
	// temp msb, temp lsb, temp CRC, humidity msb, humidity lsb, humidity CRC
	uint8_t data[5] = {0};
	
	int length;
	if(type == TEMP) length = 2; // only need first two bytes
	else length = 5;
	
	//wait until we get a valid measurement (num of bytes > 0)
	// backoff for a short period
	while(i2c_read_bytes(BUS, ADDR, &data[0], length) == 0) {
		xtimer_usleep(SHT31_MEASUREMENT_BACKOFF);
	}
	
	if(type == HUM) {
		*result = (int16_t) (((data[3] * 256) + data[4])) * 100 / 65535;
	}
	else {
		// TODO remove floating point arithmetic
		double cTemp = (((data[0] * 256) + data[1]) * 175) / 65535.0  - 45;
		
		// scale -2
		*result = (int16_t) (cTemp * 100);
	}
	
	i2c_release(BUS);
}

// value needs to be scaled 10E-2
void sht31_read_temp(const sht31_t* dev, int16_t* result) {
	_read_sensor(dev,result, TEMP);
}

void sht31_read_hum(const sht31_t* dev, int16_t* result) {
	_read_sensor(dev,result, HUM);
}
