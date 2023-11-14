/*
 * Zachary Gerhard and Monica Saad are the sole programmers of this code. To the best of our knowledge this code contains no material previously
 * published by any other person(s).
 *
 * This is an app to play a replica of the classic arcade game "PacMan".
 */

//including libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "colours.h"
#include <conio.h>

#define PACMAN 'P'
#define GHOST 'G'
#define DOT '.'
#define WALL 'W'
#define EMPTY ' '
#define UP 'w'
#define LEFT 'a'
#define DOWN 's'
#define RIGHT 'd'

//declaring global variables
// set all of map array to walls to have wall outer border
char *mapArray[11][11] = {{"W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W"},
                          {"W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W"},
                          {"W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W"},
                          {"W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W"},
                          {"W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W"},
                          {"W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W"},
                          {"W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W"},
                          {"W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W"},
                          {"W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W"},
                          {"W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W"},
                          {"W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W"}};

//declare global variables with PacMan's position in the map array and start with them set to his initial position.
int pacManRow = 5, pacManColumn = 5;
int previousPacMan[2] = {pacManRow, pacManColumn};

//declare global variables with ghost 1 and 2's positions in the map array and start with them set to their initial positions.
int ghost1Row = 1, ghost1Column = 1;
int previousGhost1[2] = {ghost1Row, ghost1Column};
int ghost2Row = 9, ghost2Column = 9;
int previousGhost2[2] = {ghost2Row, ghost2Column};

//declaring global variable to indicate whether it is ghost1's first step.
int firstStepGhost1 = 1;//set to true

//declaring global variable to indicate whether it is ghost2's first step.
int firstStepGhost2 = 1;//set to true

//declaring global variable to hold ghost1's previous direction of movement.
char ghost1PreviousDirection = 's';//initial direction of movement always down.

//declaring global variable to hold ghost2's previous direction of movement.
char ghost2PreviousDirection = 'w';//initial direction of movement always up.

//declaring global variable to hold the direction to pacman from a ghost.
char directionToPacMan;

//declaring global variable to check the spaces around the ghost for PacMan
int steps = 1;

//declaring a global variable to hold the char on the space ghost1 moves on to.
char *charHolderGhost1;

//declaring a global variable to hold the char on the space ghost2 moves on to.
char *charHolderGhost2;

//declaring global variable to hold the number of dots left on the map
int numDots = 54;

//declaring functions to check if ghosts can see pacman.
int seePacManUp(int steps, int ghostRow, int ghostColumn);
int seePacManDown(int steps, int ghostRow, int ghostColumn);
int seePacManRight(int steps, int ghostRow, int ghostColumn);
int seePacManLeft(int steps, int ghostRow, int ghostColumn);

//declaring function that checks for a wall around PacMan
int isWall(char direction, int objectRow, int objectColumn);

//declaring function that checks for a dot
int isDot(char direction);

//declaring function to find direction of ghost movement
char ghostDirection(char previousDirection, int objectRow, int objectColumn);

//declaring function to move pacman.
void pacManMove();

//declaring function to move ghosts
void ghostMove();

//declaring function to check if the player has won the game.
int winCheck(char direction);

//declaring function to print map to console.
void printMap();

//declaring a function to check if the player has lost the game.
int loseCheck();


int main() {
    char input;
    int pacManMoved;
    FILE *fp;

    //open file "map.txt" in read mode and copy into mapArray.
    fp = fopen("map.txt", "r");
    for (int i = 1; i < 10; ++i) {
        for (int j = 1; j < 10; ++j) {
            mapArray[i][j] = (char*)malloc(27*sizeof(char));
            fscanf(fp, "%s", mapArray[i][j]);
        }
    }
    //close file "map.txt"
    fclose(fp);
    printMap();

    while(1) {
        pacManMoved = 0;
        //check for keypress.
        if (kbhit() != 0) {
            //get user input.
            input = getch();
            //compare user input.
            if (input == 'w') {
                //move pacman up if no wall
                if (isWall(input, pacManRow, pacManColumn) == 0) {
                    //check if player has won
                    if (winCheck(input) == 0) {
                        pacManMoved = 1;
                    } else {
                        pacManRow -= 1;
                        pacManMove();
                        printMap();
                        printf("\nCongratulations! You win!\nPress any key to exit the game\n");
                        // exit program after next key press
                        getch();
                        return 0;
                    }
                }
                //move ghosts
                ghostMove();
                //move pacman (if not wall in his path)
                if (pacManMoved == 1) {
                    pacManRow -= 1;
                    pacManMove();
                }
                //check if player lost the game.
                if(loseCheck() == 1) {
                    getch();
                    return 0;
                }
                //print map
                printMap();
            } else if (input == 's') {
                //move pacman down if no wall
                if (isWall(input, pacManRow, pacManColumn) == 0) {
                    if (winCheck(input) == 0) {
                        pacManMoved = 1;
                    } else {
                        pacManRow += 1;
                        pacManMove();
                        printMap();
                        printf("\nCongratulations! You win!\nPress any key to exit the game\n");
                        // exit program
                        getch();
                        return 0;
                    }
                }
                ghostMove();
                if (pacManMoved == 1) {
                    pacManRow += 1;
                    pacManMove();
                }
                if(loseCheck() == 1){
                    getch();
                    return 0;
                }
                printMap();
            } else if (input == 'a') {
                //move pacman left if no wall
                if (isWall(input, pacManRow, pacManColumn) == 0) {
                    if (winCheck(input) == 0) {
                        pacManMoved = 1;
                    } else {
                        pacManColumn -= 1;
                        pacManMove();
                        printMap();
                        printf("\nCongratulations! You win!\nPress any key to exit the game\n");
                        // exit program
                        getch();
                        return 0;
                    }
                }
                ghostMove();
                if (pacManMoved == 1) {
                    pacManColumn -= 1;
                    pacManMove();
                }
                if(loseCheck() == 1){
                    getch();
                    return 0;
                }
                printMap();
            } else if (input == 'd') {
                //move pacman right if no wall
                if (isWall(input, pacManRow, pacManColumn) == 0) {
                    if (winCheck(input) == 0) {
                        pacManMoved = 1;
                    } else {
                        pacManColumn += 1;
                        pacManMove();
                        printMap();
                        printf("\nCongratulations! You win!\nPress any key to exit the game\n");
                        // exit program
                        getch();
                        return 0;
                    }
                }
                ghostMove();
                if (pacManMoved == 1) {
                    pacManColumn += 1;
                    pacManMove();
                }
                if(loseCheck() == 1){
                    getch();
                    return 0;
                }
                printMap();
            }
        }
    }
}

int seePacManUp(int steps, int ghostRow, int ghostColumn){
    if(strcmp(mapArray[ghostRow-steps][ghostColumn], "W") == 0){
        steps = 1;
        return 0;//false, can NOT see pacman up.
    }else if(strcmp(mapArray[ghostRow-steps][ghostColumn], "P") == 0){
        directionToPacMan = 'w';
        return 1;//true, can see pacman up.
    }else{
        return seePacManUp(steps+1, ghostRow, ghostColumn);
    }
}

int seePacManDown(int steps, int ghostRow, int ghostColumn){
    if(strcmp(mapArray[ghostRow+steps][ghostColumn], "W") == 0){
        steps = 1;
        return 0;//false, can NOT see pacman up.
    }else if(strcmp(mapArray[ghostRow+steps][ghostColumn], "P") == 0){
        directionToPacMan = 's';
        return 1;//true, can see pacman up.
    }else{
        return seePacManDown(steps+1, ghostRow, ghostColumn);
    }
}

int seePacManRight(int steps, int ghostRow, int ghostColumn){
    if(strcmp(mapArray[ghostRow][ghostColumn+steps], "W") == 0){
        steps = 1;
        return 0;//false, can NOT see pacman up.
    }else if(strcmp(mapArray[ghostRow][ghostColumn+steps], "P") == 0){
        directionToPacMan = 'd';
        return 1;//true, can see pacman up.
    }else{
        return seePacManRight(steps+1, ghostRow, ghostColumn);
    }
}

int seePacManLeft(int steps, int ghostRow, int ghostColumn){
    if(strcmp(mapArray[ghostRow][ghostColumn-steps], "W") == 0){
        steps = 1;
        return 0;//false, can NOT see pacman up.
    }else if(strcmp(mapArray[ghostRow][ghostColumn-steps], "P") == 0){
        directionToPacMan = 'a';
        return 1;//true, can see pacman up.
    }else{
        return seePacManLeft(steps+1, ghostRow, ghostColumn);
    }
}

//defining function to print map to console.
void printMap() {
    for (int i = 0; i < 11; ++i) {
        printf("\n");
        for (int j = 0; j < 11; ++j) {
            printf("%s  ", mapArray[i][j]);
        }
    }
}

// defining function to check if there is a dot
int isDot(char direction){
    if (direction == 'w'){
        if (strcmp(mapArray[pacManRow - 1][pacManColumn], ".") == 0) {
            return 1; //dot found = true
        } else {
            return 0; //dot found = false
        }
    }else if (direction == 's') {
        if (strcmp(mapArray[pacManRow + 1][pacManColumn], ".") == 0) {
            return 1; //dot found = true
        } else {
            return 0; //dot found = false
        }
    }else if (direction == 'a') {
        if (strcmp(mapArray[pacManRow][pacManColumn - 1], ".") == 0) {
            return 1; //dot found = true
        } else {
            return 0; //dot found = false
        }
    } else if (direction == 'd') {
        if (strcmp(mapArray[pacManRow][pacManColumn + 1], ".") == 0) {
            return 1; //dot found = true
        } else {
            return 0; //dot found = false
        }
    }
}

int winCheck(char direction){
    if(isDot(direction) == 1){
        numDots -= 1;
    }
    if(numDots == 0){
        return 1;
    }else{
        return 0;
    }
}

int loseCheck(){
    if (pacManRow == ghost1Row && pacManColumn == ghost1Column ||
        pacManRow == ghost2Row && pacManColumn == ghost2Column){
        mapArray[pacManRow][pacManColumn] = "G";
        printMap();
        printf("\nSorry, you lose. \nPress any key to exit the game\n");
        return 1;
    }
    else return 0;
}

//defining function that checks for a wall.
int isWall(char direction, int objectRow, int objectColumn){
    if (direction == 'w'){
        if (strcmp(mapArray[objectRow - 1][objectColumn], "W") == 0) {
            return 1; //wall found = true
        } else {
            return 0; //wall found = false
        }
    }else if (direction == 's') {
        if (strcmp(mapArray[objectRow + 1][objectColumn], "W") == 0) {
            return 1; //wall found = true
        } else {
            return 0; //wall found = false
        }
    }else if (direction == 'a') {
        if (strcmp(mapArray[objectRow][objectColumn - 1], "W") == 0) {
            return 1; //wall found = true
        } else {
            return 0; //wall found = false
        }
    } else if (direction == 'd') {
        if (strcmp(mapArray[objectRow][objectColumn + 1], "W") == 0) {
            return 1; //wall found = true
        } else {
            return 0; //wall found = false
        }
    }
}

//declaring function to move pacman
void pacManMove() {
    mapArray[pacManRow][pacManColumn] = "P";
    if (strcmp(mapArray[previousPacMan[0]][previousPacMan[1]], "G") == 0) {
        //do nothing
    }else{
        mapArray[previousPacMan[0]][previousPacMan[1]] = " ";
    }
    previousPacMan[0] = pacManRow;
    previousPacMan[1] = pacManColumn;
}

//defining function to determine the direction thr ghost should move.
char ghostDirection(char previousDirection, int objectRow, int objectColumn){
    steps = 0;
    if (seePacManUp(steps, objectRow, objectColumn) == 1) {
        return directionToPacMan;
    } else if (seePacManDown(steps, objectRow, objectColumn) == 1) {
        return directionToPacMan;
    } else if (seePacManRight(steps, objectRow, objectColumn) == 1) {
        return directionToPacMan;
    } else if (seePacManLeft(steps, objectRow, objectColumn) == 1) {
        return directionToPacMan;
    }else {
        if (isWall(previousDirection, objectRow, objectColumn) == 0) {
            return previousDirection;
        } else if (previousDirection == 'a') {
            if (isWall('w', objectRow, objectColumn) == 0) {
                return 'w';
            } else {
                return 's';
            }
        } else if (previousDirection == 'd') {
            if (isWall('s', objectRow, objectColumn) == 0) {
                return 's';
            } else {
                return 'w';
            }
        } else if (previousDirection == 'w') {
            if (isWall('d', objectRow, objectColumn) == 0) {
                return 'd';
            } else {
                return 'a';
            }
        } else if (previousDirection == 's') {
            if (isWall('a', objectRow, objectColumn) == 0) {
                return 'a';
            } else {
                return 'd';
            }
        }
    }
}

void ghostMove() {
    char *temp;
    //move ghost1
    if (firstStepGhost1 == 1) {
        firstStepGhost1 = 0;
        ghost1Row += 1;
        mapArray[ghost1Row][ghost1Column] = "G";
        mapArray[previousGhost1[0]][previousGhost1[1]] = ".";
        previousGhost1[0] = ghost1Row;
        previousGhost1[1] = ghost1Column;
        charHolderGhost1 = ".";
    } else {
        //find direction ghost is moving
        if (ghostDirection(ghost1PreviousDirection, ghost1Row, ghost1Column) == 'w') {
            ghost1Row -= 1;
            if (strcmp(mapArray[ghost1Row][ghost1Column], "P") == 0) {
                temp = " ";
            } else {
                temp = mapArray[ghost1Row][ghost1Column];
            }
            ghost1PreviousDirection = 'w';
        } else if (ghostDirection(ghost1PreviousDirection, ghost1Row, ghost1Column) == 's') {
            ghost1Row += 1;
            if (mapArray[ghost1Row][ghost1Column] == "P") {
                temp = " ";
            } else {
                temp = mapArray[ghost1Row][ghost1Column];
            }
            ghost1PreviousDirection = 's';
        } else if (ghostDirection(ghost1PreviousDirection, ghost1Row, ghost1Column) == 'd') {
            ghost1Column += 1;
            if (strcmp(mapArray[ghost1Row][ghost1Column], "P") == 0) {
                temp = " ";
            } else {
                temp = mapArray[ghost1Row][ghost1Column];
            }
            ghost1PreviousDirection = 'd';
        } else if (ghostDirection(ghost1PreviousDirection, ghost1Row, ghost1Column) == 'a') {
            ghost1Column -= 1;
            if (strcmp(mapArray[ghost1Row][ghost1Column], "P") == 0) {
                temp = " ";
            } else {
                temp = mapArray[ghost1Row][ghost1Column];
            }
            ghost1PreviousDirection = 'a';
        } else temp = "0";
        mapArray[ghost1Row][ghost1Column] = "G";
        mapArray[previousGhost1[0]][previousGhost1[1]] = charHolderGhost1;
        charHolderGhost1 = temp;
        previousGhost1[0] = ghost1Row;
        previousGhost1[1] = ghost1Column;
    }

    //move ghost2
    if (firstStepGhost2 == 1) {
        firstStepGhost2 = 0;
        ghost2Row -= 1;
        mapArray[ghost2Row][ghost2Column] = "G";
        mapArray[previousGhost2[0]][previousGhost2[1]] = ".";
        previousGhost2[0] = ghost2Row;
        previousGhost2[1] = ghost2Column;
        charHolderGhost2 = ".";
    } else {
        //find direction ghost is moving
        if (ghostDirection(ghost2PreviousDirection, ghost2Row, ghost2Column) == 'w') {
            ghost2Row -= 1;
            if (strcmp(mapArray[ghost2Row][ghost2Column], "P") == 0) {
                temp = " ";
            }else if(strcmp(mapArray[ghost2Row][ghost2Column], "G") == 0) {
                temp = charHolderGhost1;
            }else {
                temp = mapArray[ghost2Row][ghost2Column];
            }
            ghost2PreviousDirection = 'w';
        } else if (ghostDirection(ghost2PreviousDirection, ghost2Row, ghost2Column) == 's') {
            ghost2Row += 1;
            if (strcmp(mapArray[ghost2Row][ghost2Column], "P") == 0) {
                temp = " ";
            }else if(strcmp(mapArray[ghost2Row][ghost2Column], "G") == 0) {
                temp = charHolderGhost1;
            } else {
                temp = mapArray[ghost2Row][ghost2Column];
            }
            ghost2PreviousDirection = 's';
        } else if (ghostDirection(ghost2PreviousDirection, ghost2Row, ghost2Column) == 'd') {
            ghost2Column += 1;
            if (strcmp(mapArray[ghost2Row][ghost2Column], "P") == 0) {
                temp = " ";
            }else if(strcmp(mapArray[ghost2Row][ghost2Column], "G") == 0) {
                temp = charHolderGhost1;
            }else {
                temp = mapArray[ghost2Row][ghost2Column];
            }
            ghost2PreviousDirection = 'd';
        } else if (ghostDirection(ghost2PreviousDirection, ghost2Row, ghost2Column) == 'a') {
            ghost2Column -= 1;
            if (strcmp(mapArray[ghost2Row][ghost2Column], "P") == 0) {
                temp = " ";
            }else if(strcmp(mapArray[ghost2Row][ghost2Column], "G") == 0) {
                temp = charHolderGhost1;
            }else {
                temp = mapArray[ghost2Row][ghost2Column];
            }
            ghost2PreviousDirection = 'a';
        }
        mapArray[ghost2Row][ghost2Column] = "G";
        mapArray[previousGhost2[0]][previousGhost2[1]] = charHolderGhost2;
        charHolderGhost2 = temp;
        previousGhost2[0] = ghost2Row;
        previousGhost2[1] = ghost2Column;
    }
}