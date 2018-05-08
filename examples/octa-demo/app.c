#include "app.h"
#include <math.h>
#include <string.h>

#include "thread.h"
#include "xtimer.h"
#include "saul_reg.h"

#include "fireflyx1.h"
#include "oss7modem.h"

#define SLEEP_TIME 10
#define GPS_PORT 	OCTA_P1
#define MODEM_PORT 	OCTA_P2

#define FILE_ID_START 0x40

static char app_thread_stack[THREAD_STACKSIZE_MAIN];

void * app_thread(void * arg) {
	(void) arg; // supress warning
	
	saul_reg_t* tempSensor = saul_reg_find_type(SAUL_SENSE_TEMP);
	saul_reg_t* humSensor = saul_reg_find_type(SAUL_SENSE_HUM);
	
	while(1) {
		phydat_t result;
		
		//temperature
		saul_reg_read(tempSensor, &result);
		uint8_t tempResult = result.val[0];
		
		float floattemp = result.val[0] * pow(10,result.scale);
		printf("Temperatuur: %f\n", floattemp);
		
		// bool modem_write_file(uint8_t file_id, uint32_t offset, uint32_t size, uint8_t* data);
		modem_write_file(FILE_ID_START, 0, 1, &tempResult);
		
		// humidity
		saul_reg_read(humSensor, &result);
		uint8_t humResult = result.val[0];
		
		floattemp = result.val[0] * pow(10,result.scale);
		printf("Vochtigheid: %f\n", floattemp);
		
		modem_write_file(FILE_ID_START + 1, 0, 1, &humResult);
		
		//position
		gps_position_dd_t pos = gps_get_position_dd();
		printf("Latitude: %f\n", pos.latitude);
		printf("Longitude: %f\n", pos.longitude);
		
		int posLength = sizeof(pos);
		uint8_t data[posLength];
		memcpy(data,&pos,posLength);
		
		modem_write_file(FILE_ID_START + 2, 0, posLength, data);
		
		xtimer_sleep(SLEEP_TIME);
	}
	
}

void startApp(void) {
	gps_init_uart(GPS_PORT.uart);
	modem_init(MODEM_PORT.uart);
	
	thread_create(app_thread_stack, sizeof(app_thread_stack), THREAD_PRIORITY_MAIN - 2, 
		0 , app_thread , NULL, "App");
}
