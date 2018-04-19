
#ifndef OCTA_CONNECT_H
#define OCTA_CONNECT_H

typedef enum { RED, GREEN, BLUE} octa_led_color_t;

int octa_connect_init(void);
void octa_write_led(octa_led_color_t color, int state);

#endif
