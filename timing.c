/*
 * All timing related code here.
 */
#include <pico/stdlib.h>
#include "timing.h"

unsigned int ms_counter = 0;
unsigned int micro_counter = 0;

void timing_init(void) {
  ms_counter = 0;
  micro_counter = 0;
}

void timing_udelay(int howmuch) {
  int count;
  int count2;
  
  for (count=0;count<howmuch;count++) {
    for(count2=0;count2<8;count2++) {
    }
  }
  
}

void timing_start_timer(void) {
  ms_counter = 0;
}

void timing_stop_timer(void) {
}

void timing_reset_timer(void) {
  ms_counter = 0;
}

int timing_get_timer(void) {
  return(ms_counter);
}

/*
 * millisecond sleep routine.
 */
void timing_sleep(unsigned int dlyTicks) {
	sleep_ms(dlyTicks);
}

