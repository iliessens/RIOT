
#include "log.h"
#include "board.h"
#include "octa_auto_init.h"
#include "saul_reg.h"

/**
 * @brief   Define the number of configured sensors
 */
#define OCTA_LED_NUM 1
#define OCTA_BTN_NUM 2
#define OCTA_TOTAL_DEV_NUM (OCTA_LED_NUM + OCTA_BTN_NUM)

/**
 * @brief   Memory for the SAUL registry entries
 */
static saul_reg_t saul_entries[OCTA_TOTAL_DEV_NUM];

// device descriptor
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

	saul_entries[0].dev = NULL; //remains unused
	saul_entries[0].name = "Octa(LED)";
	saul_entries[0].driver = &octa_saul_led_driver;
	
	saul_reg_add(&(saul_entries[0]));
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
	led_init();
	btn_init();
}
