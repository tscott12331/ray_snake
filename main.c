#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include "main.h"

Color snakeCol = {
    .r = 90,
    .g = 100,
    .b = 150,
    .a = 255
};

Color cookieCol = {
    .r = 150,
    .g = 100,
    .b = 90,
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

    GameState state = {
        .paused = false,
        .gameOver = false,
        .dir = LEFT,
        .cookiePos = getNewCookiePos(head),
        .score = 0,
        .scoreText = "0"
    };

    while(!WindowShouldClose()) {
        BeginDrawing();

            checkInput(&state);
            drawBoard(head, &state);
            drawScore(&state);
            update(head, &state);

        EndDrawing();

        if(state.gameOver) {
            printf("game le over\n");
            break;
        }
    } 

    CloseWindow();

    return 0;
}

void drawScore(GameState* state) {
    DrawText(state->scoreText, BOARD_X_OFF + SCORE_PADDING, BOARD_Y_OFF + SCORE_PADDING, 15, mainTextCol);
}

void checkInput(GameState* state) {
    KeyboardKey key = GetKeyPressed();
    switch(key) {
        case KEY_W:
            state->dir = UP;
            break;
        case KEY_A:
            state->dir = LEFT;
            break;
        case KEY_S:
            state->dir = DOWN;
            break;
        case KEY_D:
            state->dir = RIGHT;
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
        newPos.x = BOARD_X_OFF + (rand() % BOARD_NUM_COLS) * 50;
        newPos.y = BOARD_Y_OFF + (rand() % BOARD_NUM_ROWS) * 50;
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

static void moveSnakeBody(Snake* head, Vector2 headPrev) {
    Vector2 movePos = headPrev;
    for(Snake* cur = head->next; cur != NULL; cur = cur->next) {
        Vector2 prevPos = cur->pos;
        cur->pos = movePos;
        movePos = prevPos; 
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
    
    moveSnakeBody(head, headPrev);  

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
    ClearBackground(DARKGRAY);

    for(Snake* cur = head; cur != NULL; cur = cur->next) {
        DrawRectangle(cur->pos.x, cur->pos.y, SNAKE_SIZE, SNAKE_SIZE, snakeCol);
    }
    
    DrawCircle(state->cookiePos.x + (SNAKE_SIZE / 2.0f), state->cookiePos.y + (SNAKE_SIZE / 2.0f), COOKIE_RAD, cookieCol);

}

Snake* initSnake() {        
    int x = GetScreenWidth()/2.0f;
    int y = GetScreenHeight()/2.0f;

    Snake* cur;
    Snake* prev = NULL; 

    for(int i = 0; i < INIT_SNAKE_LEN; i++) {
        cur = (Snake*) malloc(sizeof(Snake));
        cur->pos.x = x + (SNAKE_SIZE * i);
        cur->pos.y = y;
        cur->next = prev;

        prev = cur;
    }

    return cur;
}
