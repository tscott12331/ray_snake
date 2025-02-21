#ifndef snake_main_h
#define snake_main_h

#include "../include/raylib.h"

#define GAME_FPS 10

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

#define BOARD_WIDTH 800
#define BOARD_NUM_COLS 16
#define BOARD_NUM_ROWS 16
#define BOARD_X_OFF 0
#define BOARD_Y_OFF 0

#define BORDER_LEFT 0
#define BORDER_RIGHT WINDOW_WIDTH
#define BORDER_TOP 0
#define BORDER_BOTTOM WINDOW_HEIGHT

#define SCORE_PADDING 10

#define SNAKE_SIZE 50
#define INIT_SNAKE_LEN 3

#define COOKIE_RAD 20

typedef enum {
    LEFT,
    UP,
    RIGHT,
    DOWN
} DIR;

typedef struct {
    bool paused;
    bool gameOver;
    DIR dir;
    Vector2 cookiePos;

    int score;
    char scoreText[256];
} GameState;

typedef struct Snake {
    Vector2 pos;
    struct Snake* next;
} Snake;


Snake* initSnake();
void freeSnake(Snake* head);
void drawBoard(Snake* head, GameState* state);
void drawScore(GameState* state);
void update(Snake* head, GameState* state);
void checkInput(GameState* state);
Vector2 getNewCookiePos(Snake* head);

#endif
