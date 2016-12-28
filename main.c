/*
 * Joppy Furr - 2011
 */
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

#include "constants.h"
#include "game.h"
#include "helpers.h"
#include "timer.h"
#include "button.h"
#include "about.h"

void initialize(SDL_Surface **screen) {

    /* Start SDL */
    if(SDL_Init(SDL_INIT_EVERYTHING)) {
        fprintf(stderr, "Error initializing SDL.\n");
        exit(EXIT_FAILURE);
    }

    /* Set up the screen */
    *screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, COLOUR_DEPTH, SDL_SWSURFACE);

    if(*screen == NULL) {
        fprintf(stderr, "Error setting up screen.\n");
        exit(EXIT_FAILURE);
    }

    /* Set the window title */
    SDL_WM_SetCaption(TITLE, NULL);

    /* Start SDL_mixer */
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 512) == -1) {
        fprintf(stderr, "Error initializing SDL.\n");
        exit(EXIT_FAILURE);
    }

    if(TTF_Init() == -1) {
        fprintf(stderr, "Error initializing SDL_ttf\n");
        exit(EXIT_FAILURE);
    }
}

void cleanUp() {
    /* Tidy up everything */
    TTF_Quit();
    SDL_Quit();
}

int main(int argc, char **argv) {

    /* Surfaces */
    SDL_Surface *screen = NULL;
    SDL_Surface *background = NULL;
    SDL_Surface *titleSprites = NULL;

    /* SDL_Event structure */
    SDL_Event event;

    /* Variables */
    int quit = FALSE;
    int mouseX = 0;
    int mouseY = 0;
    int mouseClick = 0;

    /* Objects */
    Timer frameTimer;
    Button playButton;
    Button quitButton;
    Button aboutButton;

    initialize(&screen);

    /* Load images */
    background = loadImage("./media/title.png");
    titleSprites = loadImage("./media/titleSprites.png");

    /* Construct buttons */
    Button_construct(&playButton, titleSprites, 0, 0, 200, 55, screen, 373, 148);
    Button_construct(&aboutButton, titleSprites, 0, 55, 200, 43, screen, 373, 327);
    Button_construct(&quitButton, titleSprites, 0, 98, 200, 47, screen, 373, 378);

    /* Wait for the user to quit */
    /* This loop will handle the main menu */
    while(!quit) {

        /* Start the frame timer */
        Timer_start(&frameTimer);

        /* == Event handling ==  */
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

        /* Find out which button is being pointed at */
        playButton.active = inside(Button_getRect(&playButton), mouseX, mouseY);
        aboutButton.active = inside(Button_getRect(&aboutButton), mouseX, mouseY);
        quitButton.active = inside(Button_getRect(&quitButton), mouseX, mouseY);

        /* Have we pressed the play button? */
        if(playButton.active && mouseClick) {
            startGame(screen);
        }

        /* Have we pressed the about button? */
        if(aboutButton.active && mouseClick) {
            about(screen);
        }

        /* Have we pressed the quit button? */
        if(quitButton.active && mouseClick) {
            quit = TRUE;
        }

        /* == Rendering == */

        /* Render background */
        SDL_BlitSurface(background, NULL, screen, NULL);

        /* Render selected buttons, if any */
        Button_render(&playButton);
        Button_render(&aboutButton);
        Button_render(&quitButton);

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
    SDL_FreeSurface(background);
    SDL_FreeSurface(titleSprites);
    cleanUp();

    return EXIT_SUCCESS;
}

