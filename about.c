/*
 * Joppy Furr - 2011
 */
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "helpers.h"
#include "timer.h"
#include "button.h"

void about(SDL_Surface *screen) {

    /* Variables */
    int quit = FALSE;
    Timer frameTimer = { 0 };
    SDL_Event event;
    int mouseClick = 0;
    int mouseX = 0;
    int mouseY = 0;

    /* Load the images screen */
    SDL_Surface *aboutImage = NULL;
    SDL_Surface *backImage = NULL;
    aboutImage = loadImage("./media/about.png");
    backImage = loadImage("./media/back.png");

    /* Prepare a button */
    Button backButton;
    Button_construct(&backButton, backImage, 0, 0, 128, 59, screen, 13, 414);

    while(!quit) {

        /* Start the frame timer */
        Timer_start(&frameTimer);

        /* == Event handling == */
        mouseClick = 0;
        while(SDL_PollEvent(&event)) {

            if(event.type == SDL_QUIT) {
                quit = TRUE;
            }

            if(event.type == SDL_MOUSEMOTION) {
                mouseX = event.motion.x;
                mouseY = event.motion.y;
            }

            if(event.type == SDL_MOUSEBUTTONDOWN) {
                mouseX = event.button.x;
                mouseY = event.button.y;
                mouseClick++;
            }
        }

        /* == Logic == */
        backButton.active = inside(Button_getRect(&backButton), mouseX, mouseY);

        /* Have we pressed the back button? */
        if(backButton.active && mouseClick) {
            quit = TRUE;
        }

        /* == Rendering == */
        SDL_BlitSurface(aboutImage, NULL, screen, NULL);
        Button_render(&backButton);

        /* Flip screen */
        if(SDL_Flip(screen)) {
            fprintf(stderr, "Error flipping screen.\n");
            exit(EXIT_FAILURE);
        }

        /* Frame limiting */
        if(Timer_timePassed(&frameTimer) < (1000 / FRAMELIMIT)) {
            SDL_Delay(1000 / FRAMELIMIT - Timer_timePassed(&frameTimer));
        }
    }

    SDL_FreeSurface(aboutImage);
    SDL_FreeSurface(backImage);
}
