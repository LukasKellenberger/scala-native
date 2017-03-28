#ifndef TIMER_H
#define TIMER_H

#include <time.h>
#include <stdlib.h>

#define NB_INTERVALS 5

typedef struct {
    long time;
    clock_t current_start;
    long intervals[NB_INTERVALS];
    int interval_index;
} Timer;

Timer* timer_create();
void timer_start(Timer* timer);
void timer_stop(Timer* timer);
void timer_reset(Timer* timer);


#endif //TIMER_H