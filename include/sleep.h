#ifndef __sleep_h__
#define __sleep_h__

void do_nanos_sleep(time_t s, long ns);
void do_micros_sleep(time_t s, long micros);
void do_millis_sleep(time_t s, long ms);

#endif
