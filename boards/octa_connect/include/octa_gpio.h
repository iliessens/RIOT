
#ifndef OCTA_GPIO_H
#define OCTA_GPIO_H

void init_leds(void);
void init_btn(void);

typedef enum { RED, GREEN, BLUE} octa_led_color_t;

void octa_write_led(octa_led_color_t color, int state);
int octa_read_led(octa_led_color_t color);
int octa_read_button(int button);

#endif
