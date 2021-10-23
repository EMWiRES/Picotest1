/*
 * 
 */
#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include "led.h"
#include "timing.h"

const uint led_pin = 25;
unsigned char led_value = 0;

void led_init(void) {
	// Initialize CS pin high
    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);
    gpio_put(led_pin, 1);
}

void led_toggle(void) {
	led_value ^= 1;
	
	gpio_put(led_pin,led_value);
}

void led_on(void) {
	gpio_put(led_pin, 1);
}

void led_off(void) {
	gpio_put(led_pin, 0);
}

void led_show_pattern(unsigned short time,unsigned char repeats) {
  unsigned char cnt;
  for(cnt=0;cnt<repeats;cnt++) {
    timing_sleep(time);
    led_toggle();
  }
  
  led_off();
}

