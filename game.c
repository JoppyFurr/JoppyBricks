/*
 * Joppy Furr - 2011
 */
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "constants.h"
#include "game.h"
#include "helpers.h"
#include "timer.h"
#include "paddle.h"
#include "ball.h"

void startGame(SDL_Surface *screen) {

    FILE *levels = NULL;
    int i = 0;
    Game game;

    /* Game Variables and media */
    game.score = 0;
    game.lives = 3;
    game.debug = FALSE;
    game.screen = screen;
    game.blockSprites = loadImage("./media/blockSprites.png");
    game.background = loadImage("./media/backgroundTile.png");
    game.paddleSprites = loadImage("./media/paddleSprites.png");
    game.ballSprites = loadImageAlpha("./media/ballSprites.png");
    game.textColour.r = 0x00; /* Black */
    game.textColour.g = 0x00;
    game.textColour.b = 0x00;

    /* Font */
    game.scoreFont = TTF_OpenFont("./media/FreeMono.ttf", 28);
    if(game.scoreFont == NULL) {
        fprintf(stderr, "Error loading font ./media/FreeMono.ttf\n");
        exit(EXIT_FAILURE);
    }

    /* Load sound files */
    game.beep[0] = Mix_LoadWAV("./media/Beep_0.wav");
    game.beep[1] = Mix_LoadWAV("./media/Beep_1.wav");
    game.beep[2] = Mix_LoadWAV("./media/Beep_2.wav");
    game.beep[3] = Mix_LoadWAV("./media/Beep_3.wav");
    game.beep[4] = Mix_LoadWAV("./media/Beep_4.wav");
    game.beep[5] = Mix_LoadWAV("./media/Beep_5.wav");
    game.beep[6] = Mix_LoadWAV("./media/Beep_6.wav");
    game.beep[7] = Mix_LoadWAV("./media/Beep_7.wav");
    game.beep[8] = Mix_LoadWAV("./media/Beep_8.wav");
    game.beep[9] = Mix_LoadWAV("./media/Beep_9.wav");
    game.beep[10] = Mix_LoadWAV("./media/Beep_10.wav");

    for(i = 0; i < 10; i++) {
        if(game.beep[i] == NULL) {
            fprintf(stderr, "Error loading sound file ./media/Beep_%i.wav\n");
            exit(EXIT_FAILURE);
        }
    }

    /* Open levels file */
    levels = fopen("./levels.dat", "r");
    if(levels == NULL) {
        fprintf(stderr, "Error opening ./levels.dat\n");
        exit(EXIT_FAILURE);  /* We need an elegent failing mechanism... */
    }

    /* Actually play the game.. */
    while(game.lives >= 0 && openNextLevel(levels, &game)) {
        play(&game);
    }

    fclose(levels);

    /* Game over screen / high scores go here */
    if(game.lives >= 0) {
        printf("Well done. End of game\n");
    }
    else {
        printf("Too bad.. You lost the game.\n");
    }

    SDL_FreeSurface(game.background);
    SDL_FreeSurface(game.blockSprites);
    SDL_FreeSurface(game.paddleSprites);
    SDL_FreeSurface(game.ballSprites);
    TTF_CloseFont(game.scoreFont);
}

int openNextLevel(FILE *levels, Game *game) {
    int i = 0;
    int j = 0;
    int c = 0;

    if(fgets(game->levelName, 40, levels) == NULL) {
        return FALSE; /* No more levels */
    }

    if(game->levelName[0] == ':') { /* Colon marks the end of levels data */
        return FALSE;
    }

    /* We will later replace this with a proper title */
    printf("%s\n", game->levelName);

    /* levels is pointing to the start of the level data. Let's load it... */
    for(i = 0; i < 30; i++) {
        for(j = 0; j < 10; j++) {
            game->level[j][i].type = fgetc(levels) - '0';
            if(fgetc(levels) != ' ') {
                fprintf(stderr, "Error reading levels.dat: Expected space\n");
                fprintf(stderr, "Level: %s\n", game->levelName);
                fprintf(stderr, "Row: %i\n", i);
                exit(EXIT_FAILURE);
            }
            game->level[j][i].colour = fgetc(levels) - '0';

            if(fgetc(levels) == ';') { /* A comma will pass, requiring the next space.
                                          A semicolon will beak. The end of this line */
                break;
            }

            if(fgetc(levels) != ' ') {
                fprintf(stderr, "Error reading levels.dat: Expected space\n");
                fprintf(stderr, "Level: %s\n", game->levelName);
                fprintf(stderr, "Row: %i\n", i);
                exit(EXIT_FAILURE);
            }
        }
        fgetc(levels); /* Nom up the newline between rows */
    }

    return TRUE;
}

int play(Game *game) {

    /* Variables */
    int mouseClick = 0;
    int mouseX = 0;
    int mouseY = 0;
    SDL_Event event;
    SDL_Rect tempSourceRect;
    SDL_Rect tempDestRect;
    int i = 0;
    int j = 0;
    int collision = NO_COLLISION;
    int blocksLeft = 0;
    char frameUseText[16]; /* Holds "Frame use: 100%" */
    SDL_Surface *frameUseSurface = NULL;
    int lifeSpacing = 0;
    int playing = TRUE;
    int previousScore = 0;
    SDL_Surface *scoreSurface = NULL;
    char scoreText[20]; /* Should be plenty... */

    /* Objects */
    Timer frameTimer;
    Paddle paddle;
    Ball ball;

    /* Set up paddle */
    paddle.image = game->paddleSprites;
    paddle.screen = game->screen;

    /* Set up ball */
    ball.r = 8;
    ball.x = 320;
    ball.y = 240;
    ball.xVelocity = 0;
    ball.yVelocity = 0;
    ball.screen = game->screen;
    ball.image = game->ballSprites;

    sprintf(scoreText, "Score: %i", game->score);
    scoreSurface = TTF_RenderText_Blended(game->scoreFont, scoreText, game->textColour);

    while(playing && game->lives >= 0) {

        Timer_start(&frameTimer);

        /* == Event handling == */
        mouseClick = 0;
        while(SDL_PollEvent(&event)) {

            if(event.type == SDL_QUIT) {
                game->lives = -1;
                return FALSE; /* A better way to quit would be nice..
                                    Maybe the escape key.. */
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
            if(event.type == SDL_KEYDOWN) {
                switch(event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        game->lives = -1;
                        return FALSE;
                        break;
                    case SDLK_d:
                        game->debug = !(game->debug);
                        break;
                    case SDLK_s: /* Skip level */
                        if(game->debug) {
                            playing = FALSE;
                        }
                        break;
                    case SDLK_l: /* Extra life */
                        if(game->debug) {
                            game->lives += 1;
                        }
                        break;
                }
            }
        }

        /* == Logic == */
        /* Are we still playing? */
        blocksLeft = 0;
        for(i = 0; i < 30; i++) {
            for(j = 0; j < 10; j++) {
                if(game->level[j][i].type) {
                    blocksLeft++;
                }
            }
        }

        previousScore = game->score;

        if(!blocksLeft) {
            playing = FALSE;
            SDL_Delay(500); /* Give the player a pause to see that they have won the level */
        }

        if((!ball.yVelocity) && mouseClick) { /* Click to start the ball */
            ball.yVelocity = BALL_VELOCITY;
        }

        paddle.x = mouseX;

        if(Ball_checkWallCollisions(&ball) == SIDE_WALL) {
            Mix_PlayChannel(-1, game->beep[10], 0);
            ball.xVelocity = -ball.xVelocity;

        }

        if(Ball_checkWallCollisions(&ball) == TOP_WALL) {
            Mix_PlayChannel(-1, game->beep[10], 0);
            ball.yVelocity = -ball.yVelocity;
        }

        if(Ball_checkWallCollisions(&ball) == BOTTOM_WALL) {
            game->lives -= 1;
            /* Reset ball */
            ball.xVelocity = 0;
            ball.yVelocity = 0;
            ball.x = 320;
            ball.y = 240;
        }

        /* Paddle collision detection */
        if(Ball_checkPaddleCollisions(&ball, paddle.x) != NO_COLLISION) {
            /* Preserve ball velocity, but change angle depending on collision point */
            Mix_PlayChannel(-1, game->beep[10], 0);
            ball.xVelocity = BALL_VELOCITY * sin(Ball_checkPaddleCollisions(&ball, paddle.x) / 30.56);
            ball.yVelocity = -BALL_VELOCITY * cos(Ball_checkPaddleCollisions(&ball, paddle.x) / 30.56);
        }

        /* Check for collisions with the bricks */
        /* Find the closest bricks.. */
        /* This WILL be buggy. If we hit between two bricks, it will behave like hitting a corner. */
        /* Eventually, add code to check for two bricks next to one another.. If two corner collisions
         * are detected at the same time, treat as an edge, etc. */
        collision = NO_COLLISION;
        for(i = ball.y / 16 - 1; i <= ball.y / 16 + 1; i++) {
            if(i >= 0 && i < 30) {
                for(j = ball.x / 64 - 1; j <= ball.x / 64 + 1; j++) {
                    if(j >= 0 && j < 10 && collision == NO_COLLISION && game->level[j][i].type != 0) {
                        collision = Ball_checkBlockCollisions(&ball, j, i);
                        if(collision != NO_COLLISION) {
                            game->level[j][i].type = 0;
                            Mix_PlayChannel(-1, game->beep[game->level[j][i].colour], 0);
                            game->score += 10;
                        }
                    }
                }
            }
        }

        /* would a switch statement be better? */
        if(collision == CORNER) {
            ball.xVelocity = -ball.xVelocity;
            ball.yVelocity = -ball.yVelocity;
        }
        else if(collision == SIDE) {
            ball.xVelocity = -ball.xVelocity;
        }
        else if(collision == WIDTH) {
            ball.yVelocity = -ball.yVelocity;
        }

        Ball_move(&ball);

        /* == Rendering == */
        /* Render the background */
        tempDestRect.x = 0;
        tempDestRect.y = 0;
        SDL_BlitSurface(game->background, NULL, game->screen, &tempDestRect);
        tempDestRect.x = 320;
        SDL_BlitSurface(game->background, NULL, game->screen, &tempDestRect);
        tempDestRect.y = 240;
        SDL_BlitSurface(game->background, NULL, game->screen, &tempDestRect);
        tempDestRect.x = 0;
        SDL_BlitSurface(game->background, NULL, game->screen, &tempDestRect);

        /* Render the tiles */
        for(i = 0; i < 30; i++) {
            for(j = 0; j < 10; j++) {
                if(game->level[j][i].type) { /* Only type 0 is not rendered */
                    tempSourceRect.x = game->level[j][i].colour * 64;
                    tempSourceRect.w = 64;
                    tempSourceRect.y = game->level[j][i].type * 16;
                    tempSourceRect.h = 16;
                    tempDestRect.x = j * 64;
                    tempDestRect.y = i * 16;
                    SDL_BlitSurface(game->blockSprites, &tempSourceRect, game->screen, &tempDestRect);
                }
            }
        }

        /* Render the ball */
        Ball_render(&ball);

        /* Render the paddle */
        Paddle_render(&paddle);

        /* render the lives */
        tempDestRect.y = 456;
        if(game->lives > 5) {
            lifeSpacing = 8;
        }
        else {
            lifeSpacing = 20;
        }

        for(i = 0; i < game->lives; i++) {
            tempDestRect.x = 8 + i * lifeSpacing;
            SDL_BlitSurface(game->ballSprites, NULL, game->screen, &tempDestRect);
        }

        /* Score */
        if(previousScore != game->score) {
            sprintf(scoreText, "Score: %i", game->score);
            if(scoreSurface != NULL) {
                SDL_FreeSurface(scoreSurface);
            }
            scoreSurface = TTF_RenderText_Blended(game->scoreFont, scoreText, game->textColour);
        }
        SDL_BlitSurface(scoreSurface, NULL, game->screen, NULL);

        /* Frame use, before flipping screen */
        if(game->debug) {
            sprintf(frameUseText, "Frame use: %.0f%%", (100.0 * Timer_timePassed(&frameTimer)) / (1000.0 / FRAMELIMIT));
            frameUseSurface = TTF_RenderText_Solid(game->scoreFont, frameUseText, game->textColour);
            tempDestRect.x = 0;
            tempDestRect.y = 25;
            SDL_BlitSurface(frameUseSurface, NULL, game->screen, &tempDestRect);
            SDL_FreeSurface(frameUseSurface);
        }

        /* Flip screen */
        if(SDL_Flip(game->screen)) {
            fprintf(stderr, "Error flipping screen.\n");
            exit(EXIT_FAILURE);
        }

        /* Frame limiting */
        if(Timer_timePassed(&frameTimer) < (1000 / FRAMELIMIT)) {
            SDL_Delay(1000 / FRAMELIMIT - Timer_timePassed(&frameTimer));
        }
    }
    SDL_FreeSurface(scoreSurface);
    return game->lives;
}
