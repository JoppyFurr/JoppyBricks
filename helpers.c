/*
 * Joppy Furr - 2011
 */
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "helpers.h"

SDL_Surface *loadImage(char *filename) {

    SDL_Surface *initialImage;
    SDL_Surface *image;

    initialImage = IMG_Load(filename);

    if(initialImage == NULL) {
        fprintf(stderr, "Error loading image %s\n", filename);
        exit(EXIT_FAILURE);
    }

    /* Correct colour depth, etc */
    image = SDL_DisplayFormat(initialImage);

    /* Initial image is no longer needed */
    SDL_FreeSurface(initialImage);

    return image;
}

SDL_Surface *loadImageAlpha(char *filename) {

    SDL_Surface *initialImage;
    SDL_Surface *image;

    initialImage = IMG_Load(filename);

    if(initialImage == NULL) {
        fprintf(stderr, "Error loading image %s\n", filename);
        exit(EXIT_FAILURE);
    }

    /* Correct colour depth, etc */
    image = SDL_DisplayFormatAlpha(initialImage);

    /* Initial image is no longer needed */
    SDL_FreeSurface(initialImage);

    return image;
}

int inside(SDL_Rect r, int x, int y) {

    if((x >= r.x) && (x < (r.x + r.w)) && (y >= r.y) && (y < (r.y + r.h))) {
        return 1;
    }

    return 0;
}
