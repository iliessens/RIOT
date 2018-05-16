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
#include "msg.h"
#include "nmea.h"
#include "fireflyx1.h"

#define GPS_BUFFER_SIZE 512

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
static uint32_t baudrate;       // set to 57600 or higher for updaterates higher than 1Hz

static kernel_pid_t callback_pid = -1; // defines no callback

static void _process_gps_fifo(void) {
	
	//puts("GPS processing");
	
  while(fifo_get_size(&_gps_fifo) > 0) {
    uint8_t byte;
    fifo_pop(&_gps_fifo, &byte, 1);

     nmea_parse(byte);
	}

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

void *gps_rcv_thread(void *arg) {
	(void) arg;
	
	// keep thread running
	while(true) {
		mutex_lock(&gps_rcv_mutex);
		
		_process_gps_fifo();
	}
	
	return NULL;
}

bool gps_init_uart(uart_t uart_handle, int baud) {
	assert(baud != 0);
	baudrate = baud;
	
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

void process_nmea_callback(void) {
	if(callback_pid == -1) return;
	
	msg_t update_msg;
	update_msg.type = GPS_MSG_TYPE;
	
	// send message to thread
	msg_try_send(&update_msg, callback_pid);
}

void gps_set_callback(kernel_pid_t pid) {
	callback_pid = pid;
	// only init nmea callback when used
	if (pid != -1) nmea_callback_ptr = process_nmea_callback;
	else nmea_callback_ptr = NULL;
}

