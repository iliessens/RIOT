#include "board.h"  /* THIS INCLUDE IS MANDATORY */

#define SECONDS_PER_US 1000000

#ifndef OSS7MODEM_BAUDRATE
#define OSS7MODEM_BAUDRATE 115200
#endif

#ifndef OSS7MODEM_READ_TIMEOUT
#define OSS7MODEM_READ_TIMEOUT 5000 // 0.5s
#endif

#ifndef OSS7MODEM_WRITE_TIMEOUT
#define OSS7MODEM_WRITE_TIMEOUT_SECONDS 	5
#define OSS7MODEM_WRITE_TIMEOUT 			(OSS7MODEM_WRITE_TIMEOUT_SECONDS * SECONDS_PER_US)
#endif

#ifndef OSS7MODEM_UART
#define OSS7MODEM_UART UART_DEV(1)
#endif
