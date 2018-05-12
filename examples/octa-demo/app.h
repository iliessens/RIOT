#ifndef APP_H
#define APP_H

typedef enum {
	WAIT_GPS,
	WAIT_TIME,
	READ_GPS,
	READ_TEMP
} app_state_t;

app_state_t get_state(void);
int get_app_info(int argc, char** argv);
void startApp(void);

#endif
