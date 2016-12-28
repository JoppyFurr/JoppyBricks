/*
 * Joppy Furr - 2011
 */
typedef struct timerStruct {
    unsigned int startTime;
} Timer;

void Timer_start(Timer *t);

unsigned int Timer_timePassed(Timer *t);
