/*
 * Joppy Furr - 2011
 */
#include <SDL/SDL.h>

#include "button.h"

void Button_construct(Button *b, SDL_Surface *image, int spriteX, int spriteY, int spriteW, int spriteH , SDL_Surface *screen, int x, int y) {
    b->image = image;
    b->screen = screen;
    b->x = x;
    b->y =y;
    b->active = 0;

    b->sprite.x = spriteX;
    b->sprite.y = spriteY;
    b->sprite.w = spriteW;
    b->sprite.h = spriteH;
}

SDL_Rect Button_getRect(Button *b) {
    SDL_Rect r;
    r.x = b->x;
    r.y = b->y;
    r.w = b->sprite.w;
    r.h = b->sprite.h;

    return r;
}

void Button_render(Button *b) {
    SDL_Rect r = Button_getRect(b);
    if(b->active) {
        SDL_BlitSurface(b->image, &(b->sprite), b->screen, &r);
    }
}
