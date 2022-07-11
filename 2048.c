#include "2048.h"


//HELPER FUNCTIONS

//Prints the instructions for 2048!
void printInstructions() {
    printf("\e[1;1H\e[2J");
    printf("2048 is played on a 4x4 board, where each spot can hold a multiple of 2, or 0.\n");
    printf("The board can be moved up, left, down or right using the coresponding WASD keys\n");
    printf("The objective of the game is to try and find matching numbers, which can be combined by moving one equal block into another. For example: \n");
    sleep(5);
    int ** newBoard;
    
    newBoard = malloc(sizeof(int *)*4);
    for (int i = 0; i < 4; i++) {
        newBoard[i] = malloc(sizeof(int)*4);
        for (int j = 0; j < 4; j++) {
            newBoard[i][j] = 0;
            
        }
    }
    newBoard[2][2] = 2;
    newBoard[2][3] = 2;
    
    printf("\e[1;1H\e[2J");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++)
            printf("%d ", newBoard[i][j]);
        printf("\n");
    }
    printf("When we move this board right, the 2 blocks combine!\n");
    sleep(3);
    
    newBoard[2][2] = 0;
    newBoard[2][3] = 4;
    
    printf("\e[1;1H\e[2J");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++)
            printf("%d ", newBoard[i][j]);
        printf("\n");
    }
    printf("See!\n");
    sleep(3);
    printf("\e[1;1H\e[2J");
    
    printf("The game is lost when the board is full and there are no possible moves!\n");
    sleep(2);
    printf("\e[1;1H\e[2J");
    
    for (int i = 0; i < 4; i++)
        free(newBoard[i]);
    free(newBoard);
}


//This function creates a Game structure and initializes the board full of zeroes, isFull to -1, and score to 0
Game initGame() {
    Game game;
    
    game.board = malloc(sizeof(int *)*4);
    if (!game.board) exit(1);
    for (int i = 0; i < 4; i++) {
        game.board[i] = malloc(sizeof(int)*4);
        if (!game.board[i]) exit(1);
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++)
            game.board[i][j] = 0;
    }
    game.isFull = -1;
    game.score = 0;
    
    return game;
}


//This function rotates the board counter clockwise
Game rotateBoard(Game game) {
    int x = 0, y = 3;
    
    int ** newBoard;
    
    newBoard = malloc(sizeof(int *)*4);
    for (int i = 0; i < 4; i++) {
        newBoard[i] = malloc(sizeof(int)*4);
    }
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            newBoard[x][y] = game.board[r][c];
            ++x;
        }
        x = 0;
        --y;
    }
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            game.board[r][c] = newBoard[r][c];
        }
    }
    
    for (int i = 0; i < 4; i++)
        free(newBoard[i]);
    free(newBoard);
    
    return game;
}


//Copies a game into a new game
Game copyGame(Game game) {
    Game newGame;
    
    newGame.board = malloc(sizeof(int *)*4);
    if (!newGame.board) exit(1);
    for (int i = 0; i < 4; i++) {
        newGame.board[i] = malloc(sizeof(int)*4);
        if (!newGame.board[i]) exit(1);
    }
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            newGame.board[i][j] = game.board[i][j];
        }
    }
    newGame.isFull = game.isFull;
    newGame.score = game.score;
    
    return newGame;
}


//Counts the number of full spots, returning 1 if full, and 0 if not
int countFullSpots(Game game) {
    int full = 0;
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (game.board[i][j] != 0)
                ++full;
        }
    }
    if (full == 16)
        return 1;
    else
        return 0;
}


//INTERFACE FUNCTIONS

//This function is the life cycle of the game, it first initializes the game, and begins looping until there are no possible moves, ending the game ends.
void startGame() {
    int loop = 0; //had issues with board moving without input on first move
    printf("Welcome to 2048!\nDo you need a tutorial? (Y/N):");
    char input = getchar();
    if (tolower(input) == 'y')
        printInstructions();
    
    Game game = initGame();
    game = generateNum(game);
    
    sleep(1);
    while (game.isFull == 0) {
        printf("%d\n", ++loop);
        displayGame(game);

        printf("Move: \n");
        char dir;
        scanf("%s", &dir);
        game = moveBoard(game, dir);
        
        game.isFull = checkFull(game);
        game = generateNum(game);
        
    }
    printf("Game over, your final score was %d\n Thanks for playing!\n", game.score);
    freeBoard(game);
}


//When the board is empty this function generates 2 non dupicate 4's randomly, else this function generates a 2 in an empty spot
Game generateNum(Game game) {
    int r1 = rand() % 4, r2 = rand() % 4;
    
    if (game.isFull == 0) {
        while (game.board[r1][r2] != 0) {
            r1 = rand() % 4;
            r2 = rand() % 4;
        }
        game.board[r1][r2] = 2;
        return game;
    } else {
        game.board[r1][r2] = 4;
        while (game.board[r1][r2] != 0) {
            r1 = rand() % 4;
            r2 = rand() % 4;
        }
        game.board[r1][r2] = 4;
        game.isFull = 0;
        
        return game;
    }
}


//Clears the previous board and prints the current board
void displayGame(Game game){
    printf("\e[1;1H\e[2J");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (game.board[i][j] != 0)
                printf(YEL "%d " RESET, game.board[i][j]);
            else
                printf("%d ", game.board[i][j]);
            
        }
        printf("\n");
    }
}


//This function rotates the board depending on which direction the user moves the board, to where that direction is to the right. The function then moves each number as far to the right as possible, combining equal numbers (which increases score by the number that was combined), starting from the right until the board is fully moved. Before returning, the board is then rotated into its original direction
Game moveBoard(Game game, char dir) {
    printf("%c\n", dir);
    if (dir == 'w') {
        rotateBoard(game);
    } else if (dir == 'a') {
        rotateBoard(game);
        rotateBoard(game);
    } else if (dir == 's') {
        rotateBoard(game);
        rotateBoard(game);
        rotateBoard(game);
    } else if (dir != 'd')
        return game;
    
    for (int r = 0; r < 4; r++) {
        for (int c = 2; c >= 0; c--) {
            
            if (game.board[r][c] != 0) {
                
                if (c == 2) {
                    if (game.board[r][3] == 0) {
                        game.board[r][3] = game.board[r][c];
                        game.board[r][c] = 0;
                    } else if (game.board[r][3] == game.board[r][c]) {
                        game.board[r][3] *= 2;
                        game.board[r][c] = 0;
                        game.score += game.board[r][3];
                    }
                    
                } else if (c == 1) {
                    
                    if (game.board[r][3] == 0 && game.board[r][2] == 0) {
                        game.board[r][3] = game.board[r][c];
                        game.board[r][c] = 0;
                    } else if (game.board[r][3] == game.board[r][c]  && game.board[r][2] == 0) {
                        game.board[r][3] *= 2;
                        game.board[r][c] = 0;
                        game.score += game.board[r][3];
                    } else if (game.board[r][3] != 0 && game.board[r][2] == 0) {
                        game.board[r][2] = game.board[r][c];
                        game.board[r][c] = 0;
                    } else if (game.board[r][2] == game.board[r][c] && game.board[r][3] != 0) {
                        game.board[r][2] *= 2;
                        game.board[r][c] = 0;
                        game.score += game.board[r][2];
                    }
                    
                } else if (c == 0) {
                    
                    if (game.board[r][3] == 0 && game.board[r][2] == 0 && game.board[r][1] == 0) {
                        game.board[r][3] = game.board[r][c];
                        game.board[r][c] = 0;
                    } else if (game.board[r][3] == game.board[r][c] && game.board[r][2] == 0 && game.board[r][1] == 0) {
                        game.board[r][3] *= 2;
                        game.board[r][c] = 0;
                        game.score += game.board[r][3];
                    } else if (game.board[r][3] != 0 && game.board[r][2] == 0 && game.board[r][1] == 0) {
                        game.board[r][2] = game.board[r][c];
                        game.board[r][c] = 0;
                    } else if (game.board[r][3] != 0 && game.board[r][2] == game.board[r][c] && game.board[r][1] == 0) {
                        game.board[r][2] *= 2;
                        game.board[r][c] = 0;
                        game.score += game.board[r][2];
                    } else if (game.board[r][1] == 0 && game.board[r][3] != 0 && game.board[r][2] != 0) {
                        game.board[r][1] = game.board[r][c];
                        game.board[r][c] = 0;
                    } else if (game.board[r][1] == game.board[r][c] && game.board[r][3] != 0 && game.board[r][2] != 0) {
                        game.board[r][1] *= 2;
                        game.board[r][c] = 0;
                        game.score += game.board[r][1];
                    }
                }
            }
        }
    }
    
    if (dir == 's') {
        rotateBoard(game);
    } else if (dir == 'a') {
        rotateBoard(game);
        rotateBoard(game);
    } else if (dir == 'w') {
        rotateBoard(game);
        rotateBoard(game);
        rotateBoard(game);
    }
    
    return game;
}


//Checks if the board is full
int checkFull(Game game) {
    Game rightGame = copyGame(game);
    rightGame = moveBoard(rightGame, 'd');
    
    Game downGame = copyGame(game);
    downGame = moveBoard(downGame, 's');
    
    Game leftGame = copyGame(game);
    leftGame = moveBoard(leftGame, 'a');
    
    Game upGame = copyGame(game);
    upGame = moveBoard(upGame, 'w');
    
    if (countFullSpots(rightGame) == 1 && countFullSpots(downGame) == 1 && countFullSpots(leftGame) == 1 && countFullSpots(upGame) == 1)
        return 1;
    else
        return 0;
}


//Frees memory allocated for the board
void freeBoard(Game game) {
    for (int i = 0; i < 4; i++) {
        free(game.board[i]);
    }
    free(game.board);
}
