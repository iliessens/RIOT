/*
 * Copyright (C) 2017 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 *
 */

/*
 * @ingroup     auto_init_saul
 * @{
 *
 * @file
 * @brief       Auto initialization for HTS221 devices
 *
 * @author      Sebastian Meiling <s@mlng.net>
 *
 * @}
 */

#ifdef MODULE_OCTA_CONNECT

#include "log.h"
#include "octa_connect.h"

/**
 * @brief   Define the number of configured sensors
 */
#define OCTA_CONNECT_NUM      1

/**
 * @brief   Memory for the SAUL registry entries
 */
static saul_reg_t saul_entries[OCTA_CONNECT_NUM * 1];

/**
 * @brief   Reference the driver struct
 * @{
 */
extern saul_driver_t octa_saul_led_r_driver;
/** @} */

void auto_init_octa_connect(void)
{

	saul_entries[0].dev = NULL;
	saul_entries[0].name = "Octa(red)";
	saul_entries[0].driver = &octa_saul_led_r_driver;
	saul_reg_add(&(saul_entries[0]));
}

#else
typedef int dont_be_pedantic;
#endif /* MODULE_HTS221 */
