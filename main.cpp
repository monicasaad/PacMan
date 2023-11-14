/*
 * Zachary Gerhard and Monica Saad are the sole programmers of this code. To the best of our knowledge this code contains no material previously
 * published by any other person(s).
 *
 * This is an app to play a replica of the classic arcade game "PacMan".
 */

// including necessary libraries
#include <stdio.h>
#include <stdlib.h>
#include "colours.h"
#include <conio.h>

// defining values for game symbols and directions
#define PACMAN 'P'
#define GHOST 'G'
#define DOT '.'
#define WALL 'W'
#define EMPTY ' '
#define UP 'w'
#define LEFT 'a'
#define DOWN 's'
#define RIGHT 'd'

// declaring global variable to put map in an array
char** mapArray;

// declare global variables with PacMan's position in the map array and start with them set to his initial position
int pacManRow = 5, pacManColumn = 5;
// declaring variables to store PacMan's past position to be used before changing his position with user input
int previousPacMan[2] = {pacManRow, pacManColumn};

// declaring global variables with ghost 1 and 2's positions in the map array and start with them set to their initial positions
// declaring global variables for arrays to store each ghost's previous position before changing it with PacMan's movement
int ghost1Row = 1, ghost1Column = 1;
int previousGhost1[2] = {ghost1Row, ghost1Column};
int ghost2Row = 9, ghost2Column = 9;
int previousGhost2[2] = {ghost2Row, ghost2Column};

//declaring global variable to indicate whether it is ghost1's first step.
int firstStepGhost1 = 1;//set to true

//declaring global variable to indicate whether it is ghost2's first step.
int firstStepGhost2 = 1;//set to true

//declaring global variable to hold ghost1's previous direction of movement.
char ghost1PreviousDirection = DOWN;//initial direction of movement always down.

//declaring global variable to hold ghost2's previous direction of movement.
char ghost2PreviousDirection = UP;//initial direction of movement always up.

//declaring global variable to hold the direction to pacman from a ghost.
char directionToPacMan;

//declaring global variable to check the spaces around the ghost for PacMan
int steps = 1;

//declaring a global variable to hold the char on the space ghost1 moves on to.
char charHolderGhost1;

//declaring a global variable to hold the char on the space ghost2 moves on to.
char charHolderGhost2;

//declaring global variable to hold the number of dots left on the map
int numDots = 54;

//declaring functions to check if ghosts can see pacman for each direction
int seePacManUp(int steps, int ghostRow, int ghostColumn);
int seePacManDown(int steps, int ghostRow, int ghostColumn);
int seePacManRight(int steps, int ghostRow, int ghostColumn);
int seePacManLeft(int steps, int ghostRow, int ghostColumn);

//declaring function that checks if there is a wall at the targeted location on the map
int isWall(char direction, int objectRow, int objectColumn);

//declaring function that checks for a dot at the targeted location on the map
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

//main function
int main() {
    char input; // declaring variable to store input by the user
    int pacManMoved; // declaring variable to define if pacMan has moved or no (moved into the wall so position didn't change)
    FILE *fp; // declaring pointer to file

    // setting the size of the array using dynamic memory
    mapArray = (char**)malloc(11*sizeof(char*));
    for (int i = 0; i < 11; ++i) {
        mapArray[i] = (char*)malloc(11*sizeof(char));
    }

    // print that array was allocated - following sample output
    printf("Array Allocated\n");

    //open file "map.txt" in read mode and copy into mapArray.
    fp = fopen("map.txt", "r");

    // output to user if the file opened - following sample output
    if(fp == NULL){
        printf("File not found\n");
    } else {
        printf("File opened\n");
    }



    // copying file data into map array
    for (int i = 1; i < 10; ++i) {
        for (int j = 1; j < 10; ++j) {
            fscanf(fp, "%s", &mapArray[i][j]);
        }
    }
    // adding wall border around the map read from the file
    for (int i = 0; i < 11; i++){
        for (int j = 0; j < 11; j++){
            if (i == 0 || j == 0 || i == 10 || j == 10){
                mapArray[i][j] = WALL;
            }
        }
    }

    // print that map has been initialized - following sample output
    printf("Map initialized\n");

    // close file "map.txt" to free up memory
    fclose(fp);
    // print map
    printMap();

    // using while loop that is always true to insert break statements when needed to exit game
    while(1){
        // start with/reset pacManMoved = 0 to show that he hasn't moved before going through loop
        pacManMoved = 0;
        // check for keypress
        if(kbhit() != 0){
            // get user input
            input = getch();
            // if user entered 'w'
            if(input == UP){
                // check if there is a wall
                if(isWall(input, pacManRow, pacManColumn) == 0){
                    // check if player has won
                    if(winCheck(input) == 0){
                        pacManMoved = 1; // set pacManMoved to 1 if player has not won and there is no wall in the chosen direction to show that his position changed
                    } else { // if the player won execute this code:
                        pacManRow -= 1; // change the position of PacMan 1 spot up in the array
                        pacManMove(); // move PacMan
                        printMap(); // print map
                        //print win statement to user
                        printf("\nCongratulations! You win!\n Press any key to exit the game\n");
                        // exit game after user input
                        getch();
                        return 0;
                    }
                }
                // after checking for wall and if user has not won, move the ghosts
                ghostMove();

                // if the user gave an input that would move Pacman up, then change PacMan's row to one above in the array and move PacMan
                if(pacManMoved == 1){
                    pacManRow -= 1;
                    pacManMove();
                }

                // check if player lost the game
                if(loseCheck() == 1){ // if player has lost game then get input and exit
                    getch();
                    return 0;
                }
                // print map
                printMap();

                // if user entered 's'
            } else if (input == DOWN){
                // check if there is a wall
                if(isWall(input, pacManRow, pacManColumn) == 0){
                    // check if player has won
                    if(winCheck(input) == 0){
                        pacManMoved = 1; // set pacManMoved to 1 if player has not won and there is no wall in the chosen direction to show that his position changed
                    } else { // if the player won execute this code:
                        pacManRow += 1; // change the position of PacMan 1 spot down in the array
                        pacManMove(); // move PacMan
                        printMap(); // print map
                        // print win statement then get user input and exit
                        printf("\nCongratulations! You win!\nPress any key to exit the game\n");
                        getch();
                        return 0;
                    }
                }

                // after checking for wall and if user has not won, move the ghosts
                ghostMove();

                // if the user gave an input that would move Pacman down, then change PacMan's row to one below in the array and move PacMan
                if(pacManMoved == 1){
                    pacManRow += 1;
                    pacManMove();
                }

                // check if player lost the game
                if(loseCheck() == 1){ // if player has lost the game then get user input and exit
                    getch();
                    return 0;
                }
                // print map
                printMap();

                // if user input is 'a'
            } else if (input == LEFT){
                // check if there is a wall
                if (isWall(input, pacManRow, pacManColumn) == 0){
                    // check if player has won
                    if(winCheck(input) == 0){
                        pacManMoved = 1; // set pacManMoved to 1 if player has not won and there is no wall in the chosen direction to show that his position
                    } else { // if the player won execute this code:
                        pacManColumn -= 1; // change the position of PacMan 1 spot to the left in the array
                        pacManMove(); // move PacMan
                        printMap(); // print map
                        // print win statement and get user input then exit
                        printf("\nCongratulations! You win!\nPress any key to exit the game\n");
                        getch();
                        return 0;
                    }
                }

                // after checking for wall and if user has not won, move the ghosts
                ghostMove();

                // if the user gave an input that would move Pacman to the left, then change PacMan's column to one before in the array and move PacMan
                if(pacManMoved == 1){
                    pacManColumn -= 1;
                    pacManMove();
                }

                // check if player lost the game
                if(loseCheck() == 1){ // if the player lost the game then get user input then exit
                    getch();
                    return 0;
                }
                // print map
                printMap();

                // if user entered 'd'
            } else if (input == RIGHT){
                // check if there is a wall
                if (isWall(input, pacManRow, pacManColumn) == 0){
                    // check if player has won
                    if (winCheck(input) == 0){
                        pacManMoved = 1; // set pacManMoved to 1 if player has not won and there is no wall in the chosen direction to show that his position
                    } else { // if the player has won execute this code:
                        pacManColumn += 1; // change the position of PacMan 1 spot to the right in the array
                        pacManMove(); // move PacMan
                        printMap(); // print map
                        // print win statement and get user input and exit
                        printf("\nCongratulations! You win!\nPress any key to exit the game\n");
                        getch();
                        return 0;
                    }
                }

                // after checking for wall and if user has not won, move the ghosts
                ghostMove();

                // if the user gave an input that would move Pacman to the right, then change PacMan's column to one after in the array and move PacMan
                if(pacManMoved == 1){
                    pacManColumn += 1;
                    pacManMove();
                }

                // check if player lost the game
                if(loseCheck() == 1){ // if the player lost the game then get input and exit
                    getch();
                    return 0;
                }
                // print map
                printMap();

            }
        }
    }
}

// defining the function to check if ghosts can see PacMan above them
int seePacManUp(int steps, int ghostRow, int ghostColumn){
    // if the ghosts see a wall, then the function will return 0 and they will stop looking above them
    if(mapArray[ghostRow-steps][ghostColumn] == WALL){
        steps = 1;
        return 0;//false, can NOT see pacman up.
        // if the ghosts see Pacman above them, then their direction will change to UP
    } else if(mapArray[ghostRow-steps][ghostColumn] == PACMAN){
        directionToPacMan = UP;
        return 1;//true, can see pacman up.
        // if the ghosts see something else (a dot, a blank, or the other ghost), then they will keep looking at the next space up in the map until they either see PacMan or a wall
    }else{
        return seePacManUp(steps+1, ghostRow, ghostColumn);
    }
}

// defining the function to check if ghosts can see PacMan below them
int seePacManDown(int steps, int ghostRow, int ghostColumn){
    // if the ghosts see a wall, then the function will return 0 and they will stop looking below them
    if(mapArray[ghostRow+steps][ghostColumn] == WALL){
        steps = 1;
        return 0;//false, can NOT see pacman up.
        // if the ghosts see PacMan below them, then their direction will change to DOWN
    }else if(mapArray[ghostRow+steps][ghostColumn] == PACMAN){
        directionToPacMan = DOWN;
        return 1;//true, can see pacman up.
    }else{
        // if the ghosts see something else (a dot, a blank, or the other ghost), then they will keep looking at the next space down in the map until they either see PacMan or a wall
        return seePacManDown(steps+1, ghostRow, ghostColumn);
    }
}

// defining the function to check if ghosts can see PacMan to their right
int seePacManRight(int steps, int ghostRow, int ghostColumn){
    // if the ghosts see a wall, then the function will return 0 and they will stop looking below them
    if(mapArray[ghostRow][ghostColumn+steps] == WALL){
        steps = 1;
        return 0;//false, can NOT see pacman up.
        // if the ghosts see PacMan below them, then their direction will change to DOWN
    }else if(mapArray[ghostRow][ghostColumn+steps] == PACMAN){
        directionToPacMan = RIGHT;
        return 1;//true, can see pacman up.
    }else{
        // if the ghosts see something else (a dot, a blank, or the other ghost), then they will keep looking at the next space to the right in the map until they either see PacMan or a wall
        return seePacManRight(steps+1, ghostRow, ghostColumn);
    }
}

// defining the function to check if ghosts can see PacMan to their left
int seePacManLeft(int steps, int ghostRow, int ghostColumn){
    // if the ghosts see a wall, then the function will return 0 and they will stop looking below them
    if(mapArray[ghostRow][ghostColumn-steps] == WALL){
        steps = 1;
        return 0;//false, can NOT see pacman up.
        // if the ghosts see PacMan below them, then their direction will change to DOWN
    }else if(mapArray[ghostRow][ghostColumn-steps] == PACMAN){
        directionToPacMan = LEFT;
        return 1;//true, can see pacman up.
    }else{
        // if the ghosts see something else (a dot, a blank, or the other ghost), then they will keep looking at the next space to the left in the map until they either see PacMan or a wall
        return seePacManLeft(steps+1, ghostRow, ghostColumn);
    }
}

//defining function to print map to console.
void printMap() {
    // loop through map array to print it out using i to go through rows and j to go through columns
    for (int i = 0; i < 11; ++i) {
        // print a new line before each row
        printf("\n");
        for (int j = 0; j < 11; ++j) {
            // change wall colour to blue using colourChange(int) function with int values assigned to each colour as defined in colours.h
            if (mapArray[i][j] == WALL){
                colourChange(1);
                // change dot colour to white
            } else if (mapArray[i][j] == DOT){
                colourChange(15);
                // change ghost colour to pink
            } else if (mapArray[i][j] == GHOST){
                colourChange(13);
                // change PacMan's colour to yellow
            } else if (mapArray[i][j] == PACMAN){
                colourChange(6);
            }
            // print character stored at mapArray at that index with two spaces in between each character
            printf("%c  ", mapArray[i][j]);
        }
    }
    // print a new line after printing the entire map to have a line separating multiple maps printed in a row
    printf("\n");
}

// defining function to check if there is a dot
int isDot(char direction){
    if (direction == UP){
        // checking if there is a dot above PacMan
        if (mapArray[pacManRow - 1][pacManColumn] == DOT) {
            return 1; //dot found = true
        } else {
            return 0; //dot found = false
        }
    }else if (direction == DOWN) {
        // checking if there is a dot below PacMan
        if (mapArray[pacManRow + 1][pacManColumn] == DOT) {
            return 1; //dot found = true
        } else {
            return 0; //dot found = false
        }
    }else if (direction == LEFT) {
        // checking if there is a dot to the left of PacMan
        if (mapArray[pacManRow][pacManColumn - 1] == DOT) {
            return 1; //dot found = true
        } else {
            return 0; //dot found = false
        }
    } else if (direction == RIGHT) {
        // checking if there is a dot to the right of PacMan
        if (mapArray[pacManRow][pacManColumn + 1] == DOT) {
            return 1; //dot found = true
        } else {
            return 0; //dot found = false
        }
    }
}

// defining function to check if player has won
int winCheck(char direction){
    // initially number of dots is stored as a global variable
    // if there is a dot in the direction PacMan is going in, then subtract 1 from the total number of dots
    if(isDot(direction) == 1){
        numDots -= 1;
    }
    // if there are no more dots left - return 1
    if(numDots == 0){
        return 1; // player has won
    }else{
        return 0; // player has not won
    }
}

// defininf fuction to check if player has lost
int loseCheck(){
    // if PacMan's position is the same as either Ghost1 or Ghost2
    if (pacManRow == ghost1Row && pacManColumn == ghost1Column ||
        pacManRow == ghost2Row && pacManColumn == ghost2Column){
        // get 'G' to appear on top of PacMan
        mapArray[pacManRow][pacManColumn] = GHOST;
        printMap(); // print map
        // print lose statement
        printf("\nSorry, you lose. \nPress any key to exit the game\n");
        return 1; // player has lost
    }
    else return 0; // player has not lost
}

//defining function that checks for a wall.
int isWall(char direction, int objectRow, int objectColumn){
    if (direction == UP){
        // check if there is a Wall in the spot above
        if (mapArray[objectRow - 1][objectColumn] == WALL) {
            return 1; //wall found = true
        } else {
            return 0; //wall found = false
        }
    } else if (direction == DOWN) {
        // check if there is wall in the spot below
        if (mapArray[objectRow + 1][objectColumn] == WALL) {
            return 1; //wall found = true
        } else {
            return 0; //wall found = false
        }
    } else if (direction == LEFT) {
        // check if there is a wall in the spot to the left
        if (mapArray[objectRow][objectColumn - 1] == WALL) {
            return 1; //wall found = true
        } else {
            return 0; //wall found = false
        }
    } else if (direction == RIGHT) {
        // check if there is a wall in the spot to the right
        if (mapArray[objectRow][objectColumn + 1] == WALL) {
            return 1; //wall found = true
        } else {
            return 0; //wall found = false
        }
    }
}

//declaring function to move pacman
void pacManMove() {
    // print 'P' at the spot where PacMan is
    mapArray[pacManRow][pacManColumn] = PACMAN;
    // if the Ghost is directly behind PacMan (in his previous spot)
    if (mapArray[previousPacMan[0]][previousPacMan[1]] == GHOST) {
        // do nothing - not setting it to empty so it doesn't erase ghost
    }else{
        // set the spot behind PacMan to be empty (after PacMan passes on a spot it will always be empty because he is collecting all the dots)
        mapArray[previousPacMan[0]][previousPacMan[1]] = EMPTY;
    }
    // set his previous position to his current position so it updates as it loops through
    previousPacMan[0] = pacManRow;
    previousPacMan[1] = pacManColumn;
}

//defining function to determine the direction the ghost should move.
char ghostDirection(char previousDirection, int objectRow, int objectColumn){
    // resetting the number of steps the ghost is looking away from
    steps = 0;
    // if a ghost sees PacMan above it, it will go that way
    if (seePacManUp(steps, objectRow, objectColumn) == 1) {
        return directionToPacMan;
        // if a ghost sees PacMan below it, it will go that way
    } else if (seePacManDown(steps, objectRow, objectColumn) == 1) {
        return directionToPacMan;
        // if a ghost sees PacMan to its right, it will go that way
    } else if (seePacManRight(steps, objectRow, objectColumn) == 1) {
        return directionToPacMan;
        // if a ghost sees PacMan to its left, it will go that way
    } else if (seePacManLeft(steps, objectRow, objectColumn) == 1) {
        return directionToPacMan;
    }else {
        // if it doesn't see PacMan let it check for a wall and if there is no wall then let it continue in the same direction
        if (isWall(previousDirection, objectRow, objectColumn) == 0) {
            return previousDirection;
            // otherwise if there is a wall in its previous direction and it was moving to the left:
        } else if (previousDirection == LEFT) {
            // check if there is a wall above the ghost and if not then change ghost direction to up
            if (isWall(UP, objectRow, objectColumn) == 0) {
                return UP;
                // if there is a wall above it, then change ghost direction to down
            } else {
                return DOWN;
            }
            // if there is a wall in its previous direction and it was moving to the left:
        } else if (previousDirection == RIGHT) {
            // check if there is a wall below the ghost and if not then change direction to down
            if (isWall(DOWN, objectRow, objectColumn) == 0) {
                return DOWN;
                // if there is a wall below it then change its direction to up
            } else {
                return UP;
            }
            // if there is a wall in its previous direction and it was moving up:
        } else if (previousDirection == UP) {
            // check if there is a wall to its right and if not then change it direction to moving to the right
            if (isWall(RIGHT, objectRow, objectColumn) == 0) {
                return RIGHT;
                // if there is a wall to its right then change its direction to moving to the left
            } else {
                return LEFT;
            }
            // if there is a wall ints previous direction and it was moving down:
        } else if (previousDirection == DOWN) {
            // check if there is a wall to its left and if not then change its direction to moving to the left
            if (isWall(LEFT, objectRow, objectColumn) == 0) {
                return LEFT;
                // if there is a wall to its left then change its direction to moving to the right
            } else {
                return RIGHT;
            }
        }
    }
}

// defining function that moves ghosts
void ghostMove() {
    // declaring variable to temporarily store the char that is in the space the ghost is about to move onto
    char temp;
    //for ghost 1:
    // if this is the Ghost's first step (in the game)
    if (firstStepGhost1 == 1) {
        // change firstStepGhost1 to 0 to get out of loop when done
        firstStepGhost1 = 0;
        // move ghost down (starting direction)
        ghost1Row += 1;
        // set the position of the ghost on the map to 'G'
        mapArray[ghost1Row][ghost1Column] = GHOST;
        // replace the dot behind it in its previous position (this is only for first move in game)
        mapArray[previousGhost1[0]][previousGhost1[1]] = DOT;
        // update its previous position to its current position after so it continuously updates
        previousGhost1[0] = ghost1Row;
        previousGhost1[1] = ghost1Column;
        // character under Ghost 1 is dot
        charHolderGhost1 = DOT;
    } else {
        // if not first move in game
        // find the direction the ghost is moving
        // if the ghost is moving up then change its position to one above on the map
        if (ghostDirection(ghost1PreviousDirection, ghost1Row, ghost1Column) == UP) {
            ghost1Row -= 1;
            // if the position above the ghost has PacMan, the char under the ghost when it moves onto it will be blank
            if (mapArray[ghost1Row][ghost1Column] == PACMAN) {
                temp = EMPTY;
            } else {
                // if it is not PacMan, then temp will be whatever is at that coordinate
                temp = mapArray[ghost1Row][ghost1Column];
            }
            // set ghost's previous direction to up
            ghost1PreviousDirection = UP;
            // if the ghost is moving down then change its position to one below on the map
        } else if (ghostDirection(ghost1PreviousDirection, ghost1Row, ghost1Column) == DOWN) {
            ghost1Row += 1;
            // if the position below the ghost has PacMan, the char under the ghost when it moves onto it will be blank
            if (mapArray[ghost1Row][ghost1Column] == PACMAN) {
                temp = EMPTY;
            } else {
                // if it is not PacMan, then temp will be whatever is at that coordinate
                temp = mapArray[ghost1Row][ghost1Column];
            }
            // set ghost's previous direction to down
            ghost1PreviousDirection = DOWN;
            // if the ghost is moving right then change its position to one to the right on the map
        } else if (ghostDirection(ghost1PreviousDirection, ghost1Row, ghost1Column) == RIGHT) {
            ghost1Column += 1;
            // if the position to the right of the ghost has PacMan, the char under the ghost when it moves onto it will be blank
            if (mapArray[ghost1Row][ghost1Column] == PACMAN) {
                temp = EMPTY;
            } else {
                // if it is not PacMan, then temp will be whatever is at that coordinate
                temp = mapArray[ghost1Row][ghost1Column];
            }
            // set ghost's previous direction to right
            ghost1PreviousDirection = RIGHT;
            // if the ghost is moving left then change its position to one to the left on the map
        } else if (ghostDirection(ghost1PreviousDirection, ghost1Row, ghost1Column) == LEFT) {
            ghost1Column -= 1;
            // if the position to the left of the ghost has PacMan, the char under the ghost when it moves onto it will be blank
            if (mapArray[ghost1Row][ghost1Column] == PACMAN) {
                temp = EMPTY;
            } else {
                // if it is not PacMan, then temp will be whatever is at that coordinate
                temp = mapArray[ghost1Row][ghost1Column];
            }
            // set ghost's previous direction to left
            ghost1PreviousDirection = LEFT;
        } else temp = '0'; // otherwise set temp to '0' to show there is an error somewhere
        // update ghost's position printing in the map
        mapArray[ghost1Row][ghost1Column] = GHOST;
        // ghost's previous position is holding what was under the ghost
        mapArray[previousGhost1[0]][previousGhost1[1]] = charHolderGhost1;
        // what's under the ghost is being stored in temp
        charHolderGhost1 = temp;
        // update the ghost's past position to its current position so it continuously updates throughout game
        previousGhost1[0] = ghost1Row;
        previousGhost1[1] = ghost1Column;
    }

    //move ghost2
    // if this is the Ghost's first step (in the game)
    if (firstStepGhost2 == 1) {
        // change firstStepGhost1 to 0 to get out of loop when done
        firstStepGhost2 = 0;
        // move ghost up (starting direction)
        ghost2Row -= 1;
        // set the position of the ghost on the map to 'G'
        mapArray[ghost2Row][ghost2Column] = GHOST;
        // replace the dot behind it in its previous position (this is only for first move in game)
        mapArray[previousGhost2[0]][previousGhost2[1]] = DOT;
        // what's under the ghost is being stored in temp
        charHolderGhost2 = temp;
        // update the ghost's past position to its current position so it continuously updates throughout game
        previousGhost2[0] = ghost2Row;
        previousGhost2[1] = ghost2Column;
        // character under Ghost 2 is dot
        charHolderGhost2 = DOT;
    } else {
        // if not first move in game
        // find the direction the ghost is moving
        // if the ghost is moving up then change its position to one above on the map
        if (ghostDirection(ghost2PreviousDirection, ghost2Row, ghost2Column) == UP) {
            ghost2Row -= 1;
            // if the position above the ghost has PacMan, the char under the ghost when it moves onto it will be blank
            if (mapArray[ghost2Row][ghost2Column] == PACMAN) {
                temp = EMPTY;
                // if the ghosts are going through each other, set temp for ghost 2 to be the char ghost1 was just over
            }else if(mapArray[ghost2Row][ghost2Column] == GHOST) {
                temp = charHolderGhost1;
            }else {
                // if it is neither, then temp will be whatever is at that coordinate
                temp = mapArray[ghost2Row][ghost2Column];
            }
            // set ghost's previous direction to up
            ghost2PreviousDirection = UP;
            // if the ghost is moving down then change its position to one below on the map
        } else if (ghostDirection(ghost2PreviousDirection, ghost2Row, ghost2Column) == DOWN) {
            ghost2Row += 1;
            // if the position below the ghost has PacMan, the char under the ghost when it moves onto it will be blank
            if (mapArray[ghost2Row][ghost2Column] == PACMAN) {
                temp = EMPTY;
                // if the ghosts are going through each other, set temp for ghost 2 to be the char ghost1 was just over
            }else if(mapArray[ghost2Row][ghost2Column] == GHOST) {
                temp = charHolderGhost1;
            } else {
                // if it is neither, then temp will be whatever is at that coordinate
                temp = mapArray[ghost2Row][ghost2Column];
            }
            // set ghost's previous direction to down
            ghost2PreviousDirection = DOWN;
            // if the ghost is moving right then change its position to one to the right on the map
        } else if (ghostDirection(ghost2PreviousDirection, ghost2Row, ghost2Column) == RIGHT) {
            ghost2Column += 1;
            // if the position to the right of the ghost has PacMan, the char under the ghost when it moves onto it will be blank
            if (mapArray[ghost2Row][ghost2Column] == PACMAN) {
                temp = EMPTY;
                // if the ghosts are going through each other, set temp for ghost 2 to be the char ghost1 was just over
            }else if(mapArray[ghost2Row][ghost2Column] == GHOST) {
                temp = charHolderGhost1;
            }else {
                // if it is neither, then temp will be whatever is at that coordinate
                temp = mapArray[ghost2Row][ghost2Column];
            }
            // set ghost's previous position to the right
            ghost2PreviousDirection = RIGHT;
            // if the ghost is moving left then change its position to one to the left on the map
        } else if (ghostDirection(ghost2PreviousDirection, ghost2Row, ghost2Column) == LEFT) {
            ghost2Column -= 1;
            // if the position to the left of the ghost has PacMan, the char under the ghost when it moves onto it will be blank
            if (mapArray[ghost2Row][ghost2Column] == PACMAN) {
                temp = EMPTY;
                // if the ghosts are going through each other, set temp for ghost 2 to be the char ghost1 was just over
            }else if(mapArray[ghost2Row][ghost2Column] == GHOST) {
                temp = charHolderGhost1;
            }else {
                // if it is neither, then temp will be whatever is at that coordinate
                temp = mapArray[ghost2Row][ghost2Column];
            }
            // set ghost's previous direction to the left
            ghost2PreviousDirection = LEFT;
        } else temp = '0'; // otherwise set temp to '0' to show there is an error somewhere
        // update ghost's position printing in the map
        mapArray[ghost2Row][ghost2Column] = GHOST;
        // ghost's previous position is holding what was under the ghost
        mapArray[previousGhost2[0]][previousGhost2[1]] = charHolderGhost2;
        // what's under the ghost is being stored in temp
        charHolderGhost2 = temp;
        // update the ghost's past position to its current position so it continuously updates throughout game
        previousGhost2[0] = ghost2Row;
        previousGhost2[1] = ghost2Column;
    }
}