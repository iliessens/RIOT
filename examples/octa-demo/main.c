#include <stdio.h>
#include <string.h>
#include "shell.h"
#include "app.h"

const shell_command_t shell_commands[] = {
	{"appinfo","Get app info",get_app_info},
	{NULL,NULL,NULL}};

int main(void)
{
	startApp();
	
	char line_buf[SHELL_DEFAULT_BUFSIZE];
	// won't go past this normally
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
	
    return 0;
}
