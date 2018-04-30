
#ifndef OCTA_PORTS_H
#define OCTA_PORTS_H

#include "periph/i2c.h"
#include "periph/uart.h"

typedef struct {
	uart_t uart;
	i2c_t i2c;
} octa_port_t;

static const octa_port_t octa_ports[] = {
	{
		.uart 	= UART_DEV(2),
		.i2c 	= I2C_DEV(2)
	},
	{
		.uart 	= UART_DEV(1),
		.i2c	= I2C_DEV(1)
	},
	{
		.uart	= UART_DEV(0),
		.i2c	= I2C_DEV(0)
	}
};

#define OCTA_P1 octa_ports[0]
#define OCTA_P2 octa_ports[1]
#define OCTA_P3 octa_ports[2]

#endif