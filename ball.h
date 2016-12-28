/*
 * Joppy Furr - 2011
 */
#define BALL_VELOCITY 400
#define NO_COLLISION -99
#define SIDE_WALL 1
#define TOP_WALL 2
#define BOTTOM_WALL 3

#define CORNER 4
#define SIDE 5
#define WIDTH 6

typedef struct ballStruct {
    double x;
    double y;
    double r;
    double xVelocity;
    double yVelocity;
    SDL_Surface *image;
    SDL_Surface *screen;
} Ball;

void Ball_render(Ball *b);

int Ball_checkWallCollisions(Ball *b);

int Ball_checkPaddleCollisions(Ball *b, int x);

int Ball_checkBlockCollisions(Ball *b, int x, int y);
