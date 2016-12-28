/*
 * Joppy Furr - 2011
 */
#include <SDL/SDL.h>
#include "paddle.h"

void Paddle_render(Paddle *p) {

    SDL_Rect offset;

    offset.x = p->x - 32;
    offset.y = 432;

    SDL_BlitSurface(p->image, NULL, p->screen, &offset);
}
