#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#define MAX_BOARD_SIZE 50
#define NEIGHBOR_COUNT 8

/*
 NW: -1, -1 -> 1111
 N:   0, -1 -> 0011
 NE:  1, -1 -> 0111
 W:  -1,  0 -> 1100
 E:   1,  0 -> 0100
 SW: -1,  1 -> 1101
 S:   0,  1 -> 0001
 SE:  1,  1 -> 0101
 encoding: [negative][x][negative][y]
*/

//enum Neighborhood {NW=0xF, N=0x3, NE=0x7, W=0xC, E=0x4, SW=0xD, S=0x1, SE=0x5};

enum State {DEAD, LIVE};

uint8_t countNeighbors(int x, int y, bool board[][MAX_BOARD_SIZE], int boardWidth, int boardLength) {
    int count = 0;

//    if (x > 0 && y > 0)                        count += board[x-1][y-1];
//    if (y > 0)                                 count += board[x  ][y-1];
//    if (x < boardWidth-1 && y > 0)             count += board[x+1][y-1];
//    if (x > 0)                                 count += board[x-1][y  ];
//    if (x < boardWidth-1)                      count += board[x+1][y  ];
//    if (x > 0 && y < boardLength-1)            count += board[x-1][y+1];
//    if (y < boardLength-1)                     count += board[x  ][y+1];
//    if (x < boardWidth-1 && y < boardLength-1) count += board[x+1][y+1];

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

int main() {
    bool board[MAX_BOARD_SIZE][MAX_BOARD_SIZE] = {
        {0,0,0,0,0,0},
        {0,0,0,0,0,0},
        {0,0,0,0,0,0},
        {0,0,1,1,0,0},
        {0,0,0,1,0,0},
        {0,0,0,0,0,0},
        {0,0,0,0,0,0},
        {0,0,0,0,0,0}
    };
    int boardWidth = 6;
    int boardLength = 8;

    printBoard(board, boardWidth, boardLength);
    sleep(1);

    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    updateBoard(board, boardWidth, boardLength);
    printBoard(board, boardWidth, boardLength);
    sleep(1);

    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    updateBoard(board, boardWidth, boardLength);
    printBoard(board, boardWidth, boardLength);
    sleep(1);

    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    updateBoard(board, boardWidth, boardLength);
    printBoard(board, boardWidth, boardLength);
    putc('\n', stdout);

    return 0;
}
