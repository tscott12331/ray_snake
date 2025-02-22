#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include "main.h"

Color snakeCol = {
    .r = 60,
    .g = 80,
    .b = 120,
    .a = 255
};

Color headCol = {
    .r = 90,
    .g = 120,
    .b = 80,
    .a = 255
};

Color cookieCol = {
    .r = 150,
    .g = 120,
    .b = 80,
    .a = 255
};

Color mainTextCol = {
    .r = 200,
    .g = 200, 
    .b = 200,
    .a = 255
};

int main(void) {
    srand(time(NULL));

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "SNAKE");    
    SetTargetFPS(GAME_FPS);

    Snake* head = initSnake(); 
    GameState state;
    initState(head, &state);
    /*GameState state = {*/
    /*    .paused = false,*/
    /*    .gameOver = false,*/
    /*    .init = true,*/
    /*    .dir = LEFT,*/
    /*    .cookiePos = getNewCookiePos(head),*/
    /*    .score = 0,*/
    /*    .scoreText = "0"*/
    /*};*/

    while(!WindowShouldClose()) {
        BeginDrawing();

            checkInput(&state);
            if(!state.paused) {
                drawBoard(head, &state);
                drawScore(&state);
                update(head, &state);
            } else {
                drawBoard(head, &state);
                if(state.gameOver) {
                    DrawText("game over", BOARD_X_OFF + BOARD_WIDTH / 2, BOARD_Y_OFF + BOARD_HEIGHT / 2, PAUSED_FONT_SIZE, mainTextCol);
                    EndDrawing();
                    WaitTime(2);
                    freeSnake(head);
                    head = initSnake(); 
                    initState(head, &state);

                    state.paused = true;
                    
                    BeginDrawing();
                    
                }
                DrawText("paused", BOARD_X_OFF + BOARD_WIDTH / 2, BOARD_Y_OFF + BOARD_HEIGHT / 2, PAUSED_FONT_SIZE, mainTextCol);
            }

        EndDrawing();

        if(state.gameOver) {
            state.paused = true;
        }
    } 

    freeSnake(head);

    CloseWindow();

    return 0;
}

void initState(Snake* head, GameState* state) {
        state->paused = false;
        state->gameOver = false;
        state->init = true;
        state->dir = LEFT;
        state->cookiePos = getNewCookiePos(head);
        state->score = 0;
        sprintf(state->scoreText, "%d", 0);
}

void freeSnake(Snake* head) {
    Snake* tmp;
    for(Snake* cur = head; cur != NULL; ) {
        tmp = cur;
        cur = cur->next;
        free(tmp);
    }
}

void drawScore(GameState* state) {
    DrawText(state->scoreText, BOARD_X_OFF + SCORE_PADDING, BOARD_Y_OFF + SCORE_PADDING, SCORE_FONT_SIZE, mainTextCol);
}

void checkInput(GameState* state) {
    KeyboardKey key = GetKeyPressed();
    switch(key) {
        case KEY_W:
            if(state->dir == DOWN) break;
            state->dir = UP;
            break;
        case KEY_A:
            if(state->dir == RIGHT) break;
            state->dir = LEFT;
            break;
        case KEY_S:
            if(state->dir == UP) break;
            state->dir = DOWN;
            break;
        case KEY_D:
            if(state->dir == LEFT) break;
            state->dir = RIGHT;
            break;
        case KEY_SPACE:
            state->paused = !state->paused;
            break;
        default:
            break;
    }
}

static bool isInSnake(Snake* head, Vector2 pos) {
    for(Snake* cur = head; cur != NULL; cur = cur->next) {
        if(cur->pos.x == pos.x && cur->pos.y == pos.y) {
            return true;
        }
    }

    return false;
}

Vector2 getNewCookiePos(Snake* head) {
    /* NOT IMPLEMENTED */
    Vector2 newPos;
    do {
        newPos.x = BOARD_X_OFF + (rand() % BOARD_NUM_COLS) * SNAKE_SIZE;
        newPos.y = BOARD_Y_OFF + (rand() % BOARD_NUM_ROWS) * SNAKE_SIZE;
    } while(isInSnake(head, newPos));
    
    return newPos;
}

static void moveSnakeHead(Snake* head, GameState* state) {
    switch(state->dir) {
        case LEFT: {
            float newX = head->pos.x - SNAKE_SIZE;

            if(newX < BORDER_LEFT) {
                state->gameOver = true;
                return;
            }
            
            head->pos.x = newX;
            break;
        }
        case RIGHT: {
            float newX = head->pos.x + SNAKE_SIZE;

            if(newX + SNAKE_SIZE > BORDER_RIGHT) {
                state->gameOver = true;
                return;
            }
            
            head->pos.x = newX;
            break;
        }
        case UP: {
            float newY = head->pos.y - SNAKE_SIZE;

            if(newY < BORDER_TOP) {
                state->gameOver = true;
                return;
            }
            
            head->pos.y = newY;
            break;

        }
        case DOWN: {
            float newY = head->pos.y + SNAKE_SIZE;

            if(newY + SNAKE_SIZE > BORDER_BOTTOM) {
                state->gameOver = true;
                return;
            }
            
            head->pos.y = newY;
            break;
        }
    }
}

static void moveSnakeBody(Snake* head, Vector2 headPrev, GameState* state) {
    Vector2 movePos = headPrev;
    for(Snake* cur = head->next; cur != NULL; cur = cur->next) {
        Vector2 prevPos = cur->pos;
        cur->pos = movePos;
        movePos = prevPos; 
        if(head->pos.x == cur->pos.x && head->pos.y == cur->pos.y) {
            state->gameOver = true;
        }
    }
}

static void growSnake(Snake* head) {
    Snake* cur;
    for(cur = head; cur->next != NULL; cur = cur->next);

    Snake* newPiece = (Snake*) malloc(sizeof(Snake));
    newPiece->pos = cur->pos;
    newPiece->next = NULL;
    cur->next = newPiece;
}

void update(Snake* head, GameState* state) {
    Vector2 headPrev = head->pos;
    moveSnakeHead(head, state); 
    if(state->gameOver) return;

    bool ate = false;
    if(head->pos.x == state->cookiePos.x &&
        head->pos.y == state->cookiePos.y) {
        ate = true;
        sprintf(state->scoreText, "%d", ++state->score);
    }
    
    moveSnakeBody(head, headPrev, state);  
    if(state->gameOver) return;


    if(ate) {
        if(state->score + INIT_SNAKE_LEN == (BOARD_NUM_ROWS * BOARD_NUM_COLS)) {
            state->gameOver = true;
            return;
        }
        growSnake(head);
        state->cookiePos = getNewCookiePos(head);
    }
        
}

void drawBoard(Snake* head, GameState* state) {
    ClearBackground(BLACK);

    DrawRectangle(head->pos.x, head->pos.y, SNAKE_SIZE, SNAKE_SIZE, headCol);
    for(Snake* cur = head->next; cur != NULL; cur = cur->next) {
        DrawRectangle(cur->pos.x, cur->pos.y, SNAKE_SIZE, SNAKE_SIZE, snakeCol);
    }

    DrawCircle(state->cookiePos.x + (SNAKE_SIZE / 2.0f), state->cookiePos.y + (SNAKE_SIZE / 2.0f), COOKIE_RAD, cookieCol);

}

Snake* initSnake() {        
    int x = GetScreenWidth()/2.0f + (2 * SNAKE_SIZE);
    int y = GetScreenHeight()/2.0f + (2 * SNAKE_SIZE);

    Snake* cur;
    Snake* prev = NULL; 

    for(int i = 0; i < INIT_SNAKE_LEN; i++) {
        cur = (Snake*) malloc(sizeof(Snake));
        cur->pos.x = x - (SNAKE_SIZE * i);
        cur->pos.y = y;
        cur->next = prev;

        prev = cur;
    }

    return cur;
}
