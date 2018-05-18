#include <stdio.h>
#include <string.h>
#include "shell.h"
#include "app.h"
#include "periph/gpio.h"

#include "thread.h"

const shell_command_t shell_commands[] = {
	{"appinfo","Get app info",get_app_info},
	{NULL,NULL,NULL}};

int main(void)
{	
	startApp();
	
	// power down BLE
	gpio_t ble_pwr = GPIO_PIN(PORT_B,11);
	gpio_init(ble_pwr, GPIO_OUT);
	gpio_write(ble_pwr,0);
	
	char line_buf[SHELL_DEFAULT_BUFSIZE];
	// won't go past this normally
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
	
	//thread_sleep(); // alternative for shell
	
    return 0;
}
