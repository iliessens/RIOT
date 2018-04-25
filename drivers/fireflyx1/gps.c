// GPS over serial connection handling
// author: Christophe VG <contact@christophe.vg>

#include <string.h>
#include <stdbool.h>
#include <stdio.h>

//#include "types.h"
#include "fifo.h"
#include "debug.h"
#include "errors.h"
#include "scheduler.h"
#include "log.h"
#include "nmea.h"
#include "fireflyx1.h"

//#define DEBUG_GPS
//#define DEBUG_I2C
#define GPS_BUFFER_SIZE 512
#define MT3333_ADDR         0x20

typedef enum {
  INTERFACE_NOT_INITIALIZED,
  INTERFACE_UART,
  INTERFACE_I2C
} interface_t;

static uint8_t _gps_buffer[GPS_BUFFER_SIZE] = { 0 };
static fifo_t  _gps_fifo;
static bool _active = false;
static interface_t _interface = INTERFACE_NOT_INITIALIZED;
static uart_handle_t* _uart;
static i2c_handle_t* _i2c;
static pin_id_t _interrupt_pin;
static bool wait_pmtk_ack = false;
static uint32_t baudrate = 115200;       // set to 57600 or higher for updaterates higher than 1Hz
static bool baudrate_set = false;
static uint16_t updaterate = 500;       // 1000 = 1Hz, 500 = 2Hz, 200 = 5Hz, 100 = 10Hz (best to use max 2Hz)
static bool updateRate_set = false;

static void _process_gps_fifo() {
  while(fifo_get_size(&_gps_fifo) > 0) {
    uint8_t byte;
    fifo_pop(&_gps_fifo, &byte, 1);

    if(wait_pmtk_ack){
        static uint8_t pos = 0;
        const uint8_t string[] = "$PMTK";

        //log_print_string("%c", byte);
        if(string[pos] == byte) {
            if (pos == 4) {
                pos = 0;
                wait_pmtk_ack = false;
                //log_print_string("PMTK command ACKED");
                if(!baudrate_set){
                    gps_set_baudrate(baudrate);
                    baudrate_set = true;
                    log_print_string("baud rate set to %d", baudrate);
                } else{
                    if(!updateRate_set){
                        gps_set_update_rate(updaterate);
                        updateRate_set = true;
                        log_print_string("update rate set to %d", updaterate);
                    }
                }
            }
            else{
                pos++;
                return;
            }
        }
        pos = 0;
    }
    else{
        // feed GPS input to NMEA parser
        nmea_parse(byte);
    }


#ifdef DEBUG_GPS
    log_print_string("%c", byte);
#endif
  }

  sched_post_task(&_process_gps_fifo);   // continue processing
}

static void _gps_cb(uint8_t data) {
  if(! _active ) { return; }
  error_t err;
  err = fifo_put(&_gps_fifo, &data, 1);
  if (err != SUCCESS)
  {
    fifo_clear(&_gps_fifo);
   return;
  }

  if(!sched_is_scheduled(&_process_gps_fifo)) {
    sched_post_task(&_process_gps_fifo);
  }
}

static void _init() {
  fifo_init(&_gps_fifo, _gps_buffer, sizeof(_gps_buffer));
  sched_register_task(&_process_gps_fifo);
}

static bool _receive() {
  uint8_t response[255] = { 0 };
  int8_t result = i2c_read(_i2c, MT3333_ADDR, response, 255);
  fifo_put(&_gps_fifo, response, 255);

  if(result != 0) {
#ifdef DEBUG_I2C
    log_print_string("i2c_receive failed. result = %d\r\n", result);
#endif
    return false;
  }

#ifdef DEBUG_I2C
  printf("response:\r\n");
  for(uint8_t i=0; i<253; i++) {
    // three consecutive newlines means no more valid data following aka garbage
    if(response[i] == '\n' && response[i+1] == '\n' && response[i+2] == '\n') {
      log_print_string("\r\n\r\n");
      break;
    } else {
      log_print_string("%c", response[i]);
    }
  }
#endif
  return result == 0;
}

static void _poll_gps() {
  // keep fetching and processing until done and as long as receive succeeds
  // TODO: this mt333_receive call is called one too much
  while( !hw_gpio_get_in(_interrupt_pin) && _receive() ) {}

  if(!sched_is_scheduled(&_process_gps_fifo)) {
    sched_post_task(&_process_gps_fifo);
  }
}

static void _on_interrupt(pin_id_t pin_id, uint8_t event_mask) {
  assert(_interface == INTERFACE_I2C);

  if(!_active) {
    log_print_string("gps: int handler called while not active\r\n");
    return;
  }

  sched_post_task(&_poll_gps);
}

static uint16_t _compute_checksum(uint8_t* bytes, uint8_t length) {
  uint8_t sum = 0;

  do {
    sum ^= *bytes++;
  } while(--length);

  return sum;
}

static bool _send(const char* cmd) {
  assert(_interface == INTERFACE_I2C); // TODO UART not implemented
  uint8_t length = strlen(cmd);
  // construct entire cmd buffer
  uint8_t buffer[255] = { '$', 'P', 'M', 'T', 'K' };
  // copy in cmd
  memcpy(&buffer[5], cmd, length);
  // add '*'
  buffer[5+length] = '*';
  // add checksum
  uint16_t checksum = _compute_checksum(&buffer[1], 4+length);
  assert( snprintf(&buffer[5+length+1], 3, "%02x", checksum) == 2);
  // add <CR><LF>
  buffer[5+length+3] = '\r';
  buffer[5+length+4] = '\n';

#ifdef DEBUG_I2C
  log_print_string("complete cmd: ");
  for(uint8_t i=0; i<5+length+5; i++) {
    if(buffer[i] == '\n') {
      log_print_string("<lf>");
    } else if (buffer[i] == '\r') {
      log_print_string("<cr>");
    } else {
      log_print_string( "%c", buffer[i]);
    }
  }
  log_print_string("\r\n");
#endif

  int8_t result = i2c_write(_i2c, MT3333_ADDR, (uint8_t*)buffer, 5+length+5);
  if(result != 0) {
    log_print_string("i2c_send failed. result = %d\r\n", result);
  } else {
    hw_busy_wait(10000); // wait 10ms to allow slave to process cmd
  }

  return result == 0;
}

bool gps_periodic_standby(uint32_t gps_run_time, uint32_t gps_standby_time){
  assert(_interface == INTERFACE_I2C);
  bool sent=false;
  //default to normal mode
   _send("225");
  //60 second location fix time extension upon an unsuccessful fix for 3 minutes
  _send("223,1,25,180000,60000");
  char apcmd[50];
  sprintf(apcmd,"225,2,%d,%d,0,0",gps_run_time,gps_standby_time); 
  sent=_send(apcmd);
  if(sent)
    log_print_string("GPS periodic standby enabled. Run: %d, Standby: %d\n",gps_run_time,gps_standby_time);
  else 
    log_print_string("GPS periodic standby failed.\n",gps_run_time,gps_standby_time);
  return sent;
}
  


bool gps_init_uart(uart_handle_t* uart_handle) {
  assert(_interface == INTERFACE_NOT_INITIALIZED);
  _uart = uart_handle;
  _init();
  uart_set_rx_interrupt_callback(_uart, &_gps_cb);


  _interface = INTERFACE_UART;
  return true;
}

bool gps_init_i2c(i2c_handle_t* i2c_handle, pin_id_t irq_pin) {
  assert(_interface == INTERFACE_NOT_INITIALIZED);
  _i2c = i2c_handle;
  _interrupt_pin = irq_pin;
  // TODO hw_gpio_configure_pin(_interrupt_pin, true, gpioModeInput, 0);
  hw_gpio_configure_interrupt(_interrupt_pin, &_on_interrupt, GPIO_FALLING_EDGE);
  sched_register_task(&_poll_gps);

  _init();

  _interface = INTERFACE_I2C;

  return true;
}

void gps_activate() {
  assert(_interface != INTERFACE_NOT_INITIALIZED);
  if(_interface == INTERFACE_I2C) {
    hw_gpio_enable_interrupt(_interrupt_pin);
  } else if(_interface == INTERFACE_UART) {
    uart_enable(_uart);
    uart_rx_interrupt_enable(_uart);
  }

  _active = true;
}

void gps_default_settings() {
  _send("251,0");       // system default --> cold start + enter standby mode
  wait_pmtk_ack = true; // wait for PMTK ACK before sending next PMTK command
}

void gps_set_baudrate(uint32_t baudrate){
    char cmd[50];
    sprintf(cmd,"251,%d",baudrate);
    _send(cmd);  // increase baud rate (mandatory before increasing update rate)
    wait_pmtk_ack = true;
}

void gps_set_update_rate(uint16_t updateRate){
    char cmd[50];
    sprintf(cmd,"220,%d",updateRate);
    _send(cmd);  // set update rate (1000 = 1Hz, 200 = 5Hz, 100 = 10Hz)
    wait_pmtk_ack = true;
}

void gps_deactivate() {
  if(_interface == INTERFACE_I2C) {
    hw_gpio_disable_interrupt(_interrupt_pin);
  } else if(_interface == INTERFACE_UART) {
    uart_rx_interrupt_disable(_uart);
    uart_disable(_uart);
  }

  _active = false;
}

// decimal degrees = degrees + minutes/60 + seconds/3600
// N/E are positive, S/W negative

gps_position_dd_t gps_get_position_dd() {
  gps_position_dd_t dd;
  nmea_position_t   pos = nmea_get_position();
  dd.latitude = pos.latitude.deg + pos.latitude.min/60.0;
  if(pos.latitude.ns == 's') {
    dd.latitude *= -1;
  }
  dd.longitude = pos.longitude.deg + pos.longitude.min/60.0;
  if(pos.longitude.ew == 'w') {
    dd.longitude *= -1;
  }
  dd.hdop = pos.hdop;
  return dd;
}

