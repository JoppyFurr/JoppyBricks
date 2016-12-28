/*
 * Joppy Furr - 2011
 */
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <math.h>
#include "constants.h"
#include "ball.h"
#include "game.h"

void Ball_render(Ball *b) {
    SDL_Rect rect;
    rect.x = b->x - b->r;
    rect.y = b->y - b->r;

    SDL_BlitSurface(b->image, NULL, b->screen, &rect);
}

void Ball_move(Ball *b) {
    b->x += b->xVelocity / FRAMELIMIT; /* We should use the actual passed times here. */
    b->y += b->yVelocity / FRAMELIMIT;
}

int Ball_checkWallCollisions(Ball *b) { /* These magic numbers should be fixed up... */
    /* Check sides */
    if((((b->x - b->r) < 0) && (b->xVelocity < 0)) || (((b->x + b->r) >= 639) && (b->xVelocity > 0))) {
        return SIDE_WALL;
    }
    /* Check bottom */
    if((b->y + b->r) >= 479) {
        return BOTTOM_WALL;
    }
    /* Check top */
    if(((b->y - b->r) <= 0 && b->yVelocity < 0)) {
        return TOP_WALL;
    }

    return NO_COLLISION;
}

int Ball_checkPaddleCollisions(Ball *b, int x) {
    /* A hack because I haven't been bothered enough to fix the above code yet... */
    /* We have already run the above code which was false, so can make some lazy assumptions with this for now */
    double xUncertanty = 0;
    if((b->yVelocity > 0) && ((b->y + b->yVelocity / FRAMELIMIT) > 432) && ((b->y - b->yVelocity / FRAMELIMIT) < 432)) {
        /* The ball is the correct height for a collision with the paddle. Is it in the correct place? */
        if(b->xVelocity > 0) {
            xUncertanty = b->xVelocity / FRAMELIMIT;
        }
        else {
            xUncertanty = -1 * b->xVelocity / FRAMELIMIT;
        }

        if(((b->x + xUncertanty) > (x - 32 - b->r)) && ((b->x - xUncertanty) < (x + 32 +  b->r))) {
            return b->x - x;
        }

    }

    return NO_COLLISION;
}

int Ball_checkBlockCollisions(Ball *b, int x, int y) {
    int uncertanty = 0;
    uncertanty = sqrt(pow(b->xVelocity / FRAMELIMIT, 2) + pow(b->yVelocity / FRAMELIMIT, 2));

    /* Check for collisions with brick at (x, y) */
    x *= 64;
    y *= 16;

    /* First, we must find the point on the brick that is the closest to the circle */
    int closestX = 0;
    int closestY = 0;

    /* X position of closest point */
    /* Ball is to the left */
    if(b->x < x) {
        closestX = x;
    }
    /* Ball is to the right */
    else if(b->x > (x + 63)) {
        closestX = x + 63;
    }
    /* Ball is in the center */
    else {
        closestX = b->x;
    }

    /* Y position of closest point */
    /* Ball is above */
    if(b->y < y) {
        closestY = y;
    }
    /* Ball is below */
    else if(b->y > (y + 15)) {
        closestY = y + 15;
    }
    /* Ball is in the center */
    else {
        closestY = b->y;
    }

    /* If the ball was not close enough */
    if(sqrt(pow(b->x - closestX, 2) + pow(b->y - closestY, 2)) > b->r + uncertanty / 2) {
        return NO_COLLISION;
    }

    /* If we landed on a corner */
    else if(((closestX == x) && (closestY == y)) ||
            ((closestX == x) && (closestY == (y + 15))) ||
            ((closestX == (x + 63)) && (closestY == y)) ||
            ((closestX == (x + 63)) && (closestY == (y + 15)))) {
        return CORNER;
    }

    else if((closestX == x) || (closestX == (x + 63))) {
        return SIDE;
    }

    else { /* only one case left.. */
        return WIDTH;
    }
}

