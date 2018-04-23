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

#ifndef SHT31_H
#define SHT31_H

#include "periph/i2c.h"

#ifdef __cplusplus
extern "C" {
#endif

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
void sht31_read_temp(const sht31_t* dev, int16_t* data);
void sht31_read_hum(const sht31_t* dev, int16_t* data);

#ifdef __cplusplus
}
#endif

#endif /* SHT31_H */
