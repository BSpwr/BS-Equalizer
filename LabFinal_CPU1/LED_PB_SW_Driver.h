// Boris Ermakov-Spektor

#ifndef LED_PB_SW_DRIVER_H
#define LED_PB_SW_DRIVER_H

#include <F28x_Project.h>

void init_led_pb_sw_gpio();

unsigned char read_led_values(void);

bool read_led_value(unsigned char led_num);

void set_led_value(unsigned char led_num, bool val);

void set_led_values(unsigned char bits);

unsigned char read_push_buttons(void);

unsigned char read_dip_switches(void);

#endif // LED_PB_SW_DRIVER_H
