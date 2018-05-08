// GPS implementation for mt3333 based modules. Supports both UART and I2C interface
// author: Christophe VG <contact@christophe.vg>

#ifndef __GPS_H
#define __GPS_H

#include "nmea.h"
#include "periph/uart.h"

#define GPS_MSG_TYPE 1

bool gps_init_uart(uart_t uart_handle);
void gps_set_callback(kernel_pid_t pid);

//void gps_firmware_version(void);

// wrappers around nmea parser
#define gps_position_t    nmea_position_t
#define gps_have_position nmea_have_position
#define gps_get_position  nmea_get_position

typedef struct gps_position_dd {
  float latitude;
  float longitude;
  float hdop;
} gps_position_dd_t;

gps_position_dd_t gps_get_position_dd(void);

#endif
