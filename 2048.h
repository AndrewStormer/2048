#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>


#define YEL   "\x1B[33m"
#define RESET "\x1B[0m"


typedef struct game {
    int ** board;
    int isFull, score;
} Game;

//HELPER FUNCTIONS
void printInstructions();
Game initGame();
Game rotateBoard(Game);
Game copyGame(Game);
int countFullSpots(Game);

//INTERFACE FUNCTIONS
void startGame();
Game generateNum(Game);
void displayGame(Game);
Game moveBoard(Game, char);
int checkFull(Game);
void freeBoard(Game);
