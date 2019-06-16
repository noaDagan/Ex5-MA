// name: Noa Dagan
// id: 311302137

#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <strings.h>

const char MSG_ERR[150] = "Error in system call\n";
const int LEN_M = sizeof(MSG_ERR);

struct TetrisGame {
    char tetris[22][22];
    bool endGame;
    int blockRow;
    int blockCol;
    bool isHoritzional;
    int index;

};
typedef struct TetrisGame tetrisGame;

// struct parameter
tetrisGame *tetrisBlocks;

// The function check the input char and change the value accordingly
void inputChar(char input) {
    // if 'q' end the game
    if (input == 'q') {
        tetrisBlocks->endGame = true;
        return;
    } //if 'a' check and move left
    else if (input == 'a') {
        if (tetrisBlocks->isHoritzional == true) {
            if (tetrisBlocks->blockRow - 1 > 0) {
                tetrisBlocks->blockRow = tetrisBlocks->blockRow - 1;
            }
        } else {
            if (tetrisBlocks->blockRow - 2 > 0) {
                tetrisBlocks->blockRow = tetrisBlocks->blockRow - 1;
            }
        }
    } // if 'd' check and move right
    else if (input == 'd') {
        if (tetrisBlocks->isHoritzional == true) {
            if (tetrisBlocks->blockRow + 2 < 20) {
                tetrisBlocks->blockRow = tetrisBlocks->blockRow + 1;
            }
        } else {
            if (tetrisBlocks->blockRow + 3 < 20) {
                tetrisBlocks->blockRow = tetrisBlocks->blockRow + 1;
            }
        }
    } // if 's' check and  move down
    else if (input == 's') {
        if ((tetrisBlocks->blockRow - 2 > 0) &&
            (tetrisBlocks->blockRow + 3 < 20) &&
            (tetrisBlocks->blockCol > tetrisBlocks->blockRow)) {
            tetrisBlocks->blockCol = tetrisBlocks->blockCol + 1;
        }
    } // if 'w' check and change to Horitzional or not Horitzional
    else if (input == 'w') {
        if (tetrisBlocks->isHoritzional == true) {
            if ((tetrisBlocks->blockRow - 1 > 0) &&
                (tetrisBlocks->blockRow + 2 < 20) &&
                (tetrisBlocks->index <= 19)) {
                tetrisBlocks->isHoritzional = false;
            }
        } else {
            if ((tetrisBlocks->blockRow - 1 > 0) &&
                (tetrisBlocks->blockRow + 2 < 20) &&
                (tetrisBlocks->index <= 19)) {
                tetrisBlocks->isHoritzional = true;
            }
        }
    }
}


// The function send signal and alarm
void alarmS(int sigNumber) {
    signal(SIGALRM, alarmS);
}

// The function and sent char to inputChar to check the value and take care
void signalHandler(int sigNumber) {
    char input = getchar();
    inputChar(input);
}



// The function build the game and print all the block all the time
void run() {
    // clear the screen
    system("clear");
    int numOfRow = 20;
    // initalize all the value of the struct game
    int i, j;
    tetrisBlocks->endGame = false;
    tetrisBlocks->blockRow = 9;
    tetrisBlocks->index = 0;
    tetrisBlocks->blockCol = 0;
    tetrisBlocks->isHoritzional = false;
    tetrisBlocks->blockCol = 0;
    bzero(tetrisBlocks->tetris, sizeof(tetrisBlocks->tetris));
    // end less loop
    for (;;) {
        // run over all the line and coulum nad build the frame of the game
        for (i = 0; i < numOfRow; i++) {
            for (j = 0; j < numOfRow; j++) {
                if (j == 0 || j == 19 || j == 20) {
                    tetrisBlocks->tetris[i][0] = '*';
                    tetrisBlocks->tetris[i][19] = '*';
                    tetrisBlocks->tetris[i][20] = '\n';
                } else {
                    tetrisBlocks->tetris[i][j] = ' ';
                }
            }
        }
        // run over loop and build the block that move
        // fill the --- accordingly
        for (i = 0; i < numOfRow; i++) {
            if (tetrisBlocks->index == i) {
                if (tetrisBlocks->isHoritzional == false) {
                    tetrisBlocks->tetris[tetrisBlocks->index][
                            tetrisBlocks->blockRow - 1] = '-';
                    tetrisBlocks->tetris[tetrisBlocks->index][tetrisBlocks->blockRow] = '-';
                    tetrisBlocks->tetris[tetrisBlocks->index][
                            tetrisBlocks->blockRow + 1] = '-';
                } else {
                    tetrisBlocks->tetris[tetrisBlocks->index - 1][
                            tetrisBlocks->blockRow] = '-';
                    tetrisBlocks->tetris[tetrisBlocks->index][
                            tetrisBlocks->blockRow] = '-';
                    tetrisBlocks->tetris[tetrisBlocks->index + 1][
                            tetrisBlocks->blockRow] = '-';
                }
            }
            // when ---begin the bottom of screen start another one from the top
            // take care differnet if isHoritzional or not
            if (tetrisBlocks->isHoritzional == true) {
                if (tetrisBlocks->index == 19) {
                    tetrisBlocks->index = 0;
                    tetrisBlocks->blockRow = 9;
                    tetrisBlocks->blockCol = 0;
                    tetrisBlocks->isHoritzional = false;
                    break;
                }
            } else {
                if (tetrisBlocks->index == 20) {
                    tetrisBlocks->index = 0;
                    tetrisBlocks->blockRow = 9;
                    tetrisBlocks->blockCol = 0;
                    tetrisBlocks->isHoritzional = false;
                    break;
                }
            }
        }
        // write all the matrix game and the last line
        write(1, tetrisBlocks->tetris, sizeof(tetrisBlocks->tetris));
        char lastRow[21] = "********************\n";
        write(1, lastRow, sizeof(lastRow));
        tetrisBlocks->blockCol++;
        tetrisBlocks->index++;
        // pause and wait for move and clear
        signal(SIGALRM, alarmS);
        alarm(1);
        pause();
        system("clear");
        // // check if need to end the game
        if (tetrisBlocks->endGame == true) {
            return;
        }
    }
}

// main function
int main() {
    // make new to struct and initalize signal
    tetrisBlocks = (tetrisGame *) malloc(sizeof(tetrisGame *));
    signal(SIGUSR2, signalHandler);
    run();
}




