/*
 * Joppy Furr - 2011
 */
typedef struct paddleStruct {
    SDL_Surface *image;
    SDL_Surface *screen;

    int x;
} Paddle;

void Paddle_render(Paddle *p);
