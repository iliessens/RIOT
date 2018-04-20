
#ifdef MODULE_OCTA_CONNECT

#include "log.h"
#include "octa_connect.h"
#include "saul_reg.h"

/**
 * @brief   Define the number of configured sensors
 */
#define OCTA_LED_NUM 3
#define OCTA_BTN_NUM 2
#define OCTA_TOTAL_DEV_NUM (OCTA_LED_NUM + OCTA_BTN_NUM)

/**
 * @brief   Memory for the SAUL registry entries
 */
static saul_reg_t saul_entries[OCTA_TOTAL_DEV_NUM];

// device descriptor
static octa_led_t leds[OCTA_LED_NUM]; // memory for the three LEDs
static int btns[OCTA_BTN_NUM];

/**
 * @brief   Reference the driver struct
 * @{
 */
extern saul_driver_t octa_saul_led_driver;
extern saul_driver_t octa_saul_btn_driver;
/** @} */

static void led_init(void) {
	// init LEDs
	for (int i = 0; i < OCTA_LED_NUM; i++) {
		leds[i].color = i;
		
		saul_entries[i].dev = &leds[i];
		saul_entries[i].name = "Octa(LED)";
		saul_entries[i].driver = &octa_saul_led_driver;
		
		saul_reg_add(&(saul_entries[i]));
	}
}

void btn_init(void) {
	for(int i=0; i < OCTA_BTN_NUM; i++) {
		btns[i] = i + 1;
		
		saul_entries[i + OCTA_LED_NUM].dev = &btns[i];
		saul_entries[i + OCTA_LED_NUM].name = "Octa(BTN)";
		saul_entries[i + OCTA_LED_NUM].driver = &octa_saul_btn_driver;
		
		saul_reg_add(&(saul_entries[i + OCTA_LED_NUM]));
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
#endif
