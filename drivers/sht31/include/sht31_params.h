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

#ifndef SHT31_PARAMS_H
#define SHT31_PARAMS_H

#include "board.h"
#include "sht31.h"
#include "saul_reg.h"

#ifndef SHT31_PARAM_I2C
#define SHT31_PARAM_I2C     I2C_DEV(0)
#endif

#ifndef SHT31_PARAM_ADDR
#define SHT31_PARAM_ADDR    0x44
#endif

#ifndef SHT31_PARAMS
#define SHT31_PARAMS    { .i2c = SHT31_PARAM_I2C, \
                          .addr = SHT31_PARAM_ADDR }
#endif

#ifndef SHT31_SAULINFO
#define SHT31_SAULINFO { .name = "sht31" }
#endif

static const sht31_params_t sht31_params[] = {
    SHT31_PARAMS,
};

static const saul_reg_info_t sht31_saul_info[] = {
    SHT31_SAULINFO,
};

#endif
