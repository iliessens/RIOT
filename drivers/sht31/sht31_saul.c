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
#include "saul.h"
#include "sht31.h"

static int read_temp(const void * dev, phydat_t * res) {
	res->unit = UNIT_TEMP_C;
	res->scale = -2;
	
	sht31_read_temp(dev, &(res->val[0]));
	
	return 1;
}

static int read_hum(const void * dev, phydat_t * res) {
	res->unit = UNIT_PERCENT;
	res->scale = 0;
	
	sht31_read_hum(dev, &(res->val[0]));
	
	return 1;
}

const saul_driver_t sht31_saul_temp_driver = {
    .read = read_temp,
    .write = saul_notsup,
    .type = SAUL_SENSE_TEMP,
};

const saul_driver_t sht31_saul_hum_driver = {
    .read = read_hum,
    .write = saul_notsup,
    .type = SAUL_SENSE_HUM,
};