// GPS implementation for mt3333 based modules. Supports both UART and I2C interface
// author: Christophe VG <contact@christophe.vg>

#ifndef __GPS_H
#define __GPS_H

#include "nmea.h"
#include "hwuart.h"
#include "hwi2c.h"
#include "hwgpio.h"

bool gps_init_uart(uart_handle_t* uart_handle);
bool gps_init_i2c(i2c_handle_t* i2c_handle, pin_id_t interrupt_pin);

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
void gps_activate(void);
void gps_default_settings(void);
void gps_set_baudrate(uint32_t baudrate);
void gps_set_update_rate(uint16_t updateRate);
void gps_deactivate(void);

#endif
