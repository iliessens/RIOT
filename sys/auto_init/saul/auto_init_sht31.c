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

#ifdef MODULE_SHT31

#include "sht31.h"
#include "sht31_params.h"
#include "assert.h"

#define SHT31_NUM      (sizeof(sht31_params)/sizeof(sht31_params[0]))

#define SHT31_SAUL_NUM (sizeof(sht31_saul_info)/sizeof(sht31_saul_info[0]))

static sht31_t sht31_devs[SHT31_NUM];

static saul_reg_t saul_entries[SHT31_NUM * 2];

/**
 * @brief   Reference the driver struct
 * @{
 */
extern saul_driver_t sht31_saul_temp_driver;
extern saul_driver_t sht31_saul_hum_driver;

void auto_init_sht31(void) {
	assert(SHT31_SAUL_NUM == SHT31_NUM);
	
	for(unsigned int i =0; i < SHT31_NUM; i++) {
		sht31_init(&sht31_devs[i], &sht31_params[i]);
		
		saul_entries[(i * 2)].dev = &(sht31_devs[i]);
        saul_entries[(i * 2)].name = sht31_saul_info[i].name;
        saul_entries[(i * 2)].driver = &sht31_saul_temp_driver;
		
		saul_entries[(i * 2) + 1].dev = &(sht31_devs[i]);
        saul_entries[(i * 2) + 1].name = sht31_saul_info[i].name;
        saul_entries[(i * 2) + 1].driver = &sht31_saul_hum_driver;
		
		saul_reg_add(&(saul_entries[(i * 2)]));
		saul_reg_add(&(saul_entries[(i * 2) + 1]));
	}
}

#else
typedef int dont_be_pedantic;
#endif
