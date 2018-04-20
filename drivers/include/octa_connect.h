
#ifndef OCTA_CONNECT_H
#define OCTA_CONNECT_H

#include "saul_reg.h"

typedef enum { RED, GREEN, BLUE} octa_led_color_t;
typedef struct {
	octa_led_color_t color;
} octa_led_t;

int octa_connect_init(void);
void octa_write_led(octa_led_color_t color, int state);
int octa_read_led(octa_led_color_t color);
int octa_read_button(int button);

#endif
