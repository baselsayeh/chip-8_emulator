#ifndef __timers_h__
#define __timers_h__

extern volatile bool is_timers_thread_running;

void init_timers();
void timers_exit();

#endif
