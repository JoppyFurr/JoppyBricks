/*
 * Joppy Furr - 2011
 */
#include <SDL/SDL.h>

#include "timer.h"

void Timer_start(Timer *t) {
    t->startTime = SDL_GetTicks();
}

unsigned int Timer_timePassed(Timer *t) {
    return SDL_GetTicks() - t->startTime;
}
