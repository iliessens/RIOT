
#include "board.h"
#include "octa_gpio.h"
#include "octa_auto_init.h"
#include <stdio.h>
#include "periph/gpio.h"

void board_init(void) {

    /* initialize the CPU */
    cpu_init();
	
	init_leds();
	init_btn();

#ifdef AUTO_INIT_LED0
    gpio_init(LED0_PIN, GPIO_OUT);
#endif
#ifdef LED1_PIN
    gpio_init(LED1_PIN, GPIO_OUT);
#endif
#ifdef LED2_PIN
    gpio_init(LED2_PIN, GPIO_OUT);
#endif

	auto_init_octa_connect();
}
