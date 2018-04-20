
#ifdef MODULE_OCTA_CONNECT

#include "log.h"
#include "octa_connect.h"

/**
 * @brief   Define the number of configured sensors
 */
#define OCTA_CONNECT_NUM 1

/**
 * @brief   Memory for the SAUL registry entries
 */
static saul_reg_t saul_entries[OCTA_CONNECT_NUM * 5];

// device descriptor
static octa_led_t leds[3]; // memory for the three LEDs
static int btns[2];

/**
 * @brief   Reference the driver struct
 * @{
 */
extern saul_driver_t octa_saul_led_driver;
extern saul_driver_t octa_saul_btn_driver;
/** @} */

static void led_init(void) {
	// init LEDs
	for (int i = 0; i < 3; i++) {
		leds[i].color = i;
		
		saul_entries[i].dev = &leds[i];
		saul_entries[i].name = "Octa(LED)";
		saul_entries[i].driver = &octa_saul_led_driver;
		
		saul_reg_add(&(saul_entries[i]));
	}
}

void btn_init(void) {
	for(int i=0; i < 2; i++) {
		btns[i] = i + 1;
		
		saul_entries[i + 3].dev = &btns[i];
		saul_entries[i + 3].name = "Octa(BTN)";
		saul_entries[i + 3].driver = &octa_saul_btn_driver;
		
		saul_reg_add(&(saul_entries[i + 3]));
	}
}

void auto_init_octa_connect(void)
{
	octa_connect_init();
	
	led_init();
	btn_init();
}

#else
typedef int dont_be_pedantic;
#endif /* MODULE_HTS221 */
