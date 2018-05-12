#include "app.h"
#include <math.h>
#include <string.h>

#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "saul_reg.h"

#include "fireflyx1.h"
#include "oss7modem.h"

#define SLEEP_TIME 10
#define GPS_PORT 	OCTA_P1
#define MODEM_PORT 	OCTA_P2

#define FILE_ID_START 0x40

static d7ap_master_session_config_t session_config = {
    .qos = {
		
        .qos_resp_mode = SESSION_RESP_MODE_ANY,
        .qos_retry_mode = SESSION_RETRY_MODE_NO,
        .qos_stop_on_error       = false,
        .qos_record              = false
    },
    .dormant_timeout = 0,
    .addressee = {
        .ctrl = {
            .nls_method = AES_NONE,
            .id_type = ID_TYPE_NOID,
        },
		.access_class = 0x01,
        .id = {0}
    }
};

static char app_thread_stack[THREAD_STACKSIZE_MAIN];

void processSHT31(void) {
	saul_reg_t* tempSensor = saul_reg_find_type(SAUL_SENSE_TEMP);
	saul_reg_t* humSensor = saul_reg_find_type(SAUL_SENSE_HUM);
	
	phydat_t result;
	
	//temperature
	saul_reg_read(tempSensor, &result);
	uint8_t tempResult = result.val[0];
	float floattemp = result.val[0] * pow(10,result.scale);
	printf("Temperatuur: %f\n", floattemp);
	modem_send_unsolicited_response(FILE_ID_START, 0, 1, &tempResult, &session_config);
	
	
	// humidity
	saul_reg_read(humSensor, &result);
	uint8_t humResult = result.val[0];
	floattemp = result.val[0] * pow(10,result.scale);
	printf("Vochtigheid: %f\n", floattemp);
	modem_send_unsolicited_response(FILE_ID_START + 1, 0, 1, &humResult, &session_config);
	
}

void processGPS(void) {
	//position
	gps_position_dd_t pos = gps_get_position_dd();
	printf("Latitude: %f\n", pos.latitude);
	printf("Longitude: %f\n", pos.longitude);
	
	int posLength = sizeof(pos);
	uint8_t data[posLength];
	memcpy(data,&pos,posLength);
	
	//bool modem_write_file(uint8_t file_id, uint32_t offset, uint32_t size, uint8_t* data);
	modem_send_unsolicited_response(FILE_ID_START + 2, 0, posLength, data, &session_config);
}
void * app_thread(void * arg) {
	(void) arg; // supress warning
	
	while(1) {
		
		uint32_t start =  xtimer_now_usec();
		
		processSHT31();
		processGPS();
		
		msg_t msg;
		msg.type = 0; // init on undefined value
		
		while(msg.type != GPS_MSG_TYPE) {
			//TODO maybe add GPS data to IPC message
			msg_receive(&msg);
		}
		
		// new GPS data is available but may be too fast
		uint32_t time = SLEEP_TIME * 1000000 - (xtimer_now_usec() - start);
		if(time> 0) xtimer_usleep(time); // sleep remaining time
	}
	
}

void startApp(void) {
	gps_init_uart(GPS_PORT.uart);
	modem_init(MODEM_PORT.uart);
	
	kernel_pid_t pid = thread_create(app_thread_stack, sizeof(app_thread_stack), THREAD_PRIORITY_MAIN - 2, 
		0 , app_thread , NULL, "App");
	
	gps_set_callback(pid);
}
