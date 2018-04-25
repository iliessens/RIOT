// GPS over serial connection handling
// author: Christophe VG <contact@christophe.vg>

#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "types.h"
#include "fifo.h"
#include "errors.h"
#include "mutex.h"
#include "thread.h"
#include "nmea.h"
#include "fireflyx1.h"

//#define DEBUG_GPS
//#define DEBUG_I2C
#define GPS_BUFFER_SIZE 512
//#define MT3333_ADDR         0x20

typedef enum {
  INTERFACE_NOT_INITIALIZED,
  INTERFACE_UART
} interface_t;

char gps_thread_stack[THREAD_STACKSIZE_MAIN];
mutex_t gps_rcv_mutex = MUTEX_INIT_LOCKED;

static uint8_t _gps_buffer[GPS_BUFFER_SIZE] = { 0 };
static fifo_t  _gps_fifo;
static bool _active = true;
static interface_t _interface = INTERFACE_NOT_INITIALIZED;
static uart_t _uart;
static uint32_t baudrate = 115200;       // set to 57600 or higher for updaterates higher than 1Hz
//static uint16_t updaterate = 500;       // 1000 = 1Hz, 500 = 2Hz, 200 = 5Hz, 100 = 10Hz (best to use max 2Hz)

static void _process_gps_fifo(void) {
  while(fifo_get_size(&_gps_fifo) > 0) {
    uint8_t byte;
    fifo_pop(&_gps_fifo, &byte, 1);

     nmea_parse(byte);
}

  //mutex_unlock(&gps_rcv_mutex); // continue processing
  // Niet nodig want functie start opnieuw wanneer er extra data is
}

static void _gps_cb(void* arg, uint8_t data) {
	(void) arg;
	
  if(! _active ) { return; }
  error_t err;
  err = fifo_put(&_gps_fifo, &data, 1);
  if (err != SUCCESS)
  {
    fifo_clear(&_gps_fifo);
   return;
  }

	mutex_unlock(&gps_rcv_mutex);
}

/*
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
*/

/*
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
*/

/*
static uint16_t _compute_checksum(uint8_t* bytes, uint8_t length) {
  uint8_t sum = 0;

  do {
    sum ^= *bytes++;
  } while(--length);

  return sum;
}
*/

void *gps_rcv_thread(void *arg) {
	(void) arg;
	
	// keep thread running
	while(true) {
		mutex_lock(&gps_rcv_mutex);
		
		_process_gps_fifo();
	}
	
	return NULL;
}

bool gps_init_uart(uart_t uart_handle) {
  assert(_interface == INTERFACE_NOT_INITIALIZED);
  _uart = uart_handle;
  
	fifo_init(&_gps_fifo, _gps_buffer, sizeof(_gps_buffer));
	
	thread_create(gps_thread_stack, sizeof(gps_thread_stack),
                  THREAD_PRIORITY_MAIN - 1, 0,
                  gps_rcv_thread, NULL, "GPS processing");
  
  uart_init(uart_handle, baudrate, _gps_cb , NULL);


  _interface = INTERFACE_UART;
  return true;
}

// decimal degrees = degrees + minutes/60 + seconds/3600
// N/E are positive, S/W negative

gps_position_dd_t gps_get_position_dd(void) {
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

