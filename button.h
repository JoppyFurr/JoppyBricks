/*
 * Joppy Furr - 2011
 */
typedef struct buttonStruct {
    SDL_Surface *image;
    SDL_Rect sprite; /* Position on spritesheet */
    SDL_Surface *screen;
    int x; /* Origin */
    int y;
    int active;
} Button;

void Button_construct(Button *b, SDL_Surface *image, int spriteX, int spriteY, int spriteW, int spriteH, SDL_Surface *screen, int x, int y);

SDL_Rect Button_getRect(Button *b);

void Button_render(Button *b);
