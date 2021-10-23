#ifndef __TIMING_H__
#define __TIMING_H__
void timing_init(void);
void timing_udelay(int howmuch);
void timing_start_timer(void);
void timing_stop_timer(void);
void timing_reset_timer(void);
int timing_get_timer(void);
void timing_sleep(unsigned int dlyTicks);
#endif
