#include <stdio.h>
#include <stdint.h>
#include <ncurses.h>
#include <time.h>

#define MAX_BOARD_SIZE 50
#define NEIGHBOR_COUNT 8
#define ITERATIONS 10
#define DRAW_CHAR 'X'
#define EMPTY_CHAR ' '
#define SPEED 300000000 // 0.3s

enum State {DEAD, LIVE};

uint8_t countNeighbors(int x, int y, bool board[][MAX_BOARD_SIZE], int boardWidth, int boardLength) {
    int count = 0;

    if (x > 0 && y > 0)                         count += board[y-1][x-1];
    if (y > 0)                                  count += board[y-1][x];
    if (x < boardWidth-1 && y > 0)              count += board[y-1][x+1];
    if (x > 0)                                  count += board[y][x-1];
    if (x < boardWidth-1)                       count += board[y][x+1];
    if (x > 0 && y < boardLength-1)             count += board[y+1][x-1];
    if (y < boardLength-1)                      count += board[y+1][x];
    if (x < boardWidth-1 && y < boardLength-1)  count += board[y+1][x+1];

    return count;
}

void printBoard(bool board[][MAX_BOARD_SIZE], int boardWidth, int boardLength) {
    for (int i = 0; i < boardLength; i++) {
        for (int j = 0; j < boardWidth; j++) {
            printf("%d", board[i][j]);
        }
        putc('\n', stdout);
    }
}

bool cellNext(int x, int y, bool board[][MAX_BOARD_SIZE], int boardWidth, int boardLength) {
    int neighborCount = countNeighbors(x, y, board, boardWidth, boardLength);

    if (board[y][x] == LIVE && (neighborCount < 2 || neighborCount > 3)) {
        return 0;
    }
    else if (board[y][x] == DEAD && neighborCount == 3) {
        return 1;
    }
    else {
        return board[y][x];
    }
}

void copyBoard(bool destBoard[][MAX_BOARD_SIZE], bool sourceBoard[][MAX_BOARD_SIZE], int boardWidth, int boardLength) {
    for (int i = 0; i < boardLength; i++) {
        for (int j = 0; j < boardWidth; j++) {
            destBoard[i][j] = sourceBoard[i][j];
        }
    }
}

void updateBoard(bool board[][MAX_BOARD_SIZE], int boardWidth, int boardLength) {
    bool bufferBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
    copyBoard(bufferBoard, board, boardWidth, boardLength);

    for (int i = 0; i < boardLength; i++) {
        for (int j = 0; j < boardWidth; j++) {
            board[i][j] = cellNext(j, i, bufferBoard, boardWidth, boardLength);
        }
    }
}

void drawBoard(bool board[][MAX_BOARD_SIZE], int boardWidth, int boardLength) {
    for (int i = 0; i < boardLength; i++) {
        for (int j = 0; j < boardWidth; j++) {
            move(i, j);
            if (board[i][j] == LIVE) {
                printw("%c", DRAW_CHAR);
            }
            else if (board[i][j] == DEAD) {
                printw("%c", EMPTY_CHAR);
            }
            refresh();
        }
    }
}

int main() {
    bool board[MAX_BOARD_SIZE][MAX_BOARD_SIZE] = {
        {0,0,0,0,0,0},
        {0,0,0,0,0,0},
        {0,0,0,0,0,0},
        {0,0,0,0,0,0},
        {0,1,1,1,0,0},
        {0,0,0,1,0,0},
        {0,0,1,0,0,0},
        {0,0,0,0,0,0}
    };
    int boardWidth = 6;
    int boardLength = 8;
    struct timespec req;
    req.tv_sec = 0;
    req.tv_nsec = SPEED;

    // Ncurses setup
    initscr();
    curs_set(0);

    // Draw and update board ITERATIONS times
    for (int i = 0; i < ITERATIONS; i++) {
        drawBoard(board, boardWidth, boardLength);

        // Show current board state for SPEED nanoseconds
        nanosleep(&req, NULL);
        updateBoard(board, boardWidth, boardLength);
    }

    // Indicate done
    move(0,0);
    printw("done.");
    getch();

    // Ncurses cleanup
    endwin();

    return 0;
}
