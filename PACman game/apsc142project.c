// APSC 142 Engineering Programming Project Starter Code
// Copyright Sean Kauffman 2024

// don't forget to update your project configuration to select "Emulate terminal in the output console"

// Make sure to include all relevant libraries
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// colours.h contains functions to change text colour and read single characters without requiring an enter
#include "colours.h"
// defines.h contains useful definitions to keep your code readable
#include "defines.h"
// map.h, game.h, and ghost.h contain prototypes of functions you must implement
#include <tgmath.h>

#include "map.h"
#include "game.h"
#include "actor.h"

// These global variables must be used to store map information.
// Almost every function needs these variables, so keeping them as globals helps keep things organized.
// map is a pointer to a dynamically allocated map for displaying to the user
// dot_map is a pointer to a dynamically allocated map for keeping track of what dots are left
char *map = NULL, *dot_map = NULL;
// width and height store the width and height of map, NOT counting outer walls
int width, height;


/**
 * This is the hardcoded map used for labs 1-3.
 * Once you implement load_map in lab 4 you should remove this map, as it cannot be used for the
 * final version of the project.
 * The map's dimensions are 9x9
 */
#define HARDCODED_WIDTH 9
#define HARDCODED_HEIGHT 9
char hardcoded_map[] = {
        GHOST, DOT, DOT, DOT, WALL, DOT, DOT, DOT, EMPTY,
        DOT, WALL, WALL, DOT, WALL, DOT, WALL, WALL, DOT,
        DOT, WALL, DOT, DOT, DOT, DOT, DOT, WALL, DOT,
        DOT, WALL, DOT, WALL, WALL, WALL, DOT, WALL, DOT,
        DOT, DOT, DOT, DOT, PLAYER, DOT, DOT, DOT, DOT,
        DOT, WALL, DOT, WALL, WALL, WALL, DOT, WALL, DOT,
        DOT, WALL, DOT, DOT, DOT, DOT, DOT, WALL, DOT,
        DOT, WALL, WALL, DOT, WALL, DOT, WALL, WALL, DOT,
        EMPTY, DOT, DOT, DOT, WALL, DOT, DOT, DOT, GHOST
};

// You are NOT allowed to add more global variables!
// The globals that are included in the starter code provide a convenient way to share information
// between the many functions that all need access to the same data, but all other data must be
// passed using function arguments.  If you add new globals, the autograder tests will fail to run
// and you will not receive a good mark.

/**
 * Updates the positions of the ghosts.
 * For a passed ghosts, this function checks if they can see the
 * player and moves them accordingly.  If a ghost can see the player,
 * it must move towards the player.  Otherwise, the ghost will move
 * in a random valid direction (not into a wall).  If the ghost is
 * trapped by walls in all four directions, it should give up and
 * not move.
 *
 * The ghosts_y and ghosts_x arrays are meant to hold NUM_GHOSTS
 * coordinates.  So, index 0 of ghosts_x holds the X coordinate
 * of ghost 0 and index 0 of ghosts_y holds the Y coordinate of
 * ghost 0.
 * @param player_y The Y coordinate of player
 * @param player_x The X coordinate of player
 * @param ghost_y A pointer to the Y coordinates of a ghost
 * @param ghost_x A pointer to the X coordinates of a ghost
 * @return
 */

//from the lab 4 video
void update_ghost(int player_y, int player_x, int * ghost_y, int * ghost_x);

// define a return code for a memory error
#define ERR_MEMORY 4

/**
 * Main entry point into your program.
 * Make sure that main returns appropriate status codes depending on what
 * happens.  The codes you must use are:
 *   NO_ERROR when no error occurs
 *   ERR_NO_MAP when no map file is found
 *   ERR_NO_PLAYER when no player is found on the map
 *   ERR_NO_GHOSTS when fewer than 2 ghosts are found on the map
 *
 * Make sure that any allocated memory is freed before returning.
 * @return a status code
 */

int main(void) {
    // This ensures that printf writes to the console immediately, instead of buffering.
    // If you remove this, you will not be able to rely on printf to work if your program crashes.
    setbuf(stdout, NULL);

    // set the map to point to the hardcoded map
    width = HARDCODED_WIDTH;
    height = HARDCODED_HEIGHT;

    // we don't actually have to use dot_map for lab1
    map = hardcoded_map;

    //loads map, copied from the lab 4 guidance video
    map=load_map("map.txt",&height,&width);

    //if map not working, return this code (ERR_NO_MAP)
    if(map==NULL) return ERR_NO_MAP;

    //count for player and ghost
    //if values r not correct, output either error code, depending on situation (ERR_NO_PLAYER or ERR_NO_GHOSTS
    //declare variables
    int playerCount = 0, ghostCount = 0;

    //loop thru map to count number of ghosts and players
    for (int i=0; i<height*width; i++) {
        if (map[i]=='P')playerCount++; //player counter
        if (map[i]=='G')ghostCount++;//ghost counter
    }

    //cjeck if criteria matches
    //if true, missing player or <2 ghosts outputs
    if (playerCount<1) return ERR_NO_PLAYER;
    if (ghostCount<2) return ERR_NO_GHOSTS;

    // allocate space for the dots
    dot_map = malloc(width * height);
    if (dot_map == NULL) {
        return ERR_MEMORY;
    }
    // copy dots into dot_map
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (map[y * width + x] == DOT) {
                dot_map[y * width + x] = DOT;
            } else {
                dot_map[y * width + x] = EMPTY;
            }
        }
    }

    // input holds the user input
    char input = 0;
    // we need the player position - we can hardcode it for now
    //not hardcoded anymore
    int player_x;
    //declare x of player variable
    int player_y;
    //declare x of player variable

    for (int i=0;i<height*width;i++) {    //i to go thru all map[i] spots
        if (map[i]==PLAYER) {
            player_y=i/width;
            player_x=i%width;
        }
    }

    // we also need the ghosts positions - we can again hardcode them and array hold x and y for ghost
    int ghostsX[NUM_GHOSTS];
    //declare array for ghost 1 and 2's x
    int ghostsY[NUM_GHOSTS];
    //declare array for ghost 1 and 2's y
    int ghost_index=0; // track ghost

    //find ghost loacition and put in xy
    for (int j = 0; j<NUM_GHOSTS; j++) {
        for (int i=0+(j*(ghost_index+1)); i<width*height; i++) {
            if (map[i]==GHOST) {
                ghostsX[j]=i%width; //math works bc we are remaindering (total spots moved%width of each row=x coordinate)
                ghostsY[j]=i/width; //math wroks bc we are dividing (totalspots moved/width of each row=y coordinate)
                ghost_index=i;
                break;
            }
        }
    }


    // loop until we hit the end of input
    while (input != EOF) {
        // print the map
        print_map();

        // get a character - blocks until one is input
        input = getch();

        // move the player
        move_player(&player_y, &player_x, input);

        // update the ghosts
        for (int ghost = 0; ghost < NUM_GHOSTS; ghost++) {
            update_ghost(player_y, player_x, &ghostsY[ghost], &ghostsX[ghost]);
        }

        //check for loss condition
        if (check_loss(player_y, player_x, ghostsY, ghostsX) == YOU_LOSE) {
            //print losing map and print loss output, then break to end code
            print_map();
            printf("Sorry, you lose.\n");
            break;
        }

        //check for win condition
        if (check_win() == YOU_WIN) {
            //print winning map and print win output, then break to end code
            print_map();
            printf("Congratulations! You win!\n");
            break;
        }
    }  // quit if we hit the end of input

    // free the dot_map
    free(dot_map);

    free(map);
    //added from video for lab 4

    // You must return the correct error code from defines.h from main depending on what happened
    return NO_ERROR;
}


void update_ghost(int player_y, int player_x, int * ghost_y, int * ghost_x) {
    // check if the ghost can see the player
    char direction = sees_player(player_y, player_x, *ghost_y, *ghost_x);

    // if the ghost doesn't see the player
    if (direction == SEES_NOTHING) {
        // get a random direction, keeping track of if we already tried it
        char tried_directions[] = {0,0,0,0};// track tried direction
        char direction_map[] = {UP, LEFT, DOWN, RIGHT};//map directions
        int move_result;//declare movement variable for future use
        do {
            // get a random direction
            int direction_int = rand() % 4;
            // set that we tried the direction
            tried_directions[direction_int] = -1;
            // get the input direction to move
            direction = direction_map[direction_int];
            // try it!
            move_result = move_ghost(ghost_y, ghost_x, direction);
            // if we try all the directions, bail out - we can't move
        } while (move_result != MOVED_OKAY &&
                 (!tried_directions[0] || !tried_directions[1] || !tried_directions[2] || !tried_directions[3])); //loop while cannot move in the given direction
    } else if (direction != EATING_PLAYER) {
        // if the ghost does see the player (but is not eating them), move in that direction
        move_ghost(ghost_y, ghost_x, direction);
    }
}
