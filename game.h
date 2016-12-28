/*
 * Joppy Furr - 2011
 */

typedef struct blockStruct {
    char type;
    char colour;
} Block;

typedef struct gameStruct {

    /* Variables */
    int score;
    int lives;
    int debug;


    SDL_Surface *screen;
    SDL_Color textColour;

    /* Level data */
    Block level[10][30];
    char levelName[40];

    /* Sprite sheets */
    SDL_Surface *background;
    SDL_Surface *blockSprites;
    SDL_Surface *paddleSprites;
    SDL_Surface *ballSprites;

    /* Fonts */
    TTF_Font *titleFont;
    TTF_Font *scoreFont;

    /* Sounds */
    Mix_Chunk *beep[11];

} Game;

void startGame(SDL_Surface *screen);

/* Read the next un-read level into the levels array.
If there are no more levels, return 0, otherwise return 1. */
/* We should also show a title for the level here */
int openNextLevel(FILE *levels, Game *game);

/* Play the currently loaded level, if the player wins, return the number of lives.
 * If the player looses, return -1 */
int play(Game *game);
