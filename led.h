#ifndef __LED_H__
#define __LED_H__
void led_init(void);
void led_toggle(void);
void led_on(void);
void led_off(void);

void led_show_pattern(unsigned short time,unsigned char repeats);
#endif
