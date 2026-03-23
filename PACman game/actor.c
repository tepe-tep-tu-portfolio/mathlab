
// APSC 142 Engineering Programming Project Starter Code
// Copyright Sean Kauffman 2024

#include "defines.h"
#include "actor.h"
#include "map.h"   // included for is_wall

extern char * map, * dot_map; //share variable
extern int height; //row
extern int width; // columns

char sees_player(int player_y, int player_x, int ghost_y, int ghost_x) {
    // check to see if the ghost and player in same spot

    if (player_y == ghost_y && player_x == ghost_x) return EATING_PLAYER;

    //if there colum in vertical line
    if (player_x == ghost_x) {
        int step = (player_y > ghost_y) ? 1 : -1; //down for below ,up for above
        for (int y = ghost_y + step; y != player_y; y += step) {
            if (map[y * width + ghost_x] == WALL) return SEES_NOTHING; //stop if there are wall
        }
        return (step == 1) ? DOWN : UP; //clear
    }
    //if there in row in horizon
    if (player_y == ghost_y) {
        int step = (player_x > ghost_x) ? 1 : -1; //right or left for player
        for (int x = ghost_x + step; x != player_x; x += step) {
            if (map[ghost_y * width + x] == WALL) return SEES_NOTHING; //stop if there re wall
        }
        return (step == 1) ? RIGHT : LEFT;//clear
    }
    return SEES_NOTHING; // if not in same row and column ghost see nothing
}
// move player in direction
int move_player(int * y, int * x, char direction) {

    int new_y = *y; // put y and x in temp *y and *x
    int new_x = *x;

    switch (direction) {
        case LEFT:  new_x--; break; //move left down x
        case RIGHT: new_x++; break;//move right more x
        case UP:    new_y--; break;//move up down y
        case DOWN:  new_y++; break;//move down up y
        default: return MOVED_INVALID_DIRECTION; //if invalid input, wasd
    }

    //check if it moves out of map
    if (new_y < 0 || new_y >= height || new_x < 0 || new_x >= width) return MOVED_WALL;
    //check if moved into wall
    if (map[new_y * width + new_x] == WALL) return MOVED_WALL;
    map[new_y * width + new_x] = PLAYER; // updates map
    dot_map[new_y * width + new_x] = EMPTY; //set empty in the new position on map
    map[*y * width + *x] = EMPTY; //set empty in the new position on dot map (dot eaten)
    //update temp *y*x to new y x
    *y = new_y;
    *x = new_x;
    return MOVED_OKAY;
}
// ghost move
int move_ghost(int * y, int * x, char direction) {

    int new_y = *y; //y in temp cariable
    int new_x = *x; //x in temp variable

    //give direction
    switch (direction) {
        case LEFT:  new_x--; break; //if valid direction entered to game wasd?
        case RIGHT: new_x++; break;
        case UP:    new_y--; break;
        case DOWN:  new_y++; break;
        default: return MOVED_INVALID_DIRECTION;//default: aka if it cant go any of the above, it is invalid
    }

    //chekc move is outside?
    if (new_y < 0 || new_y >= height || new_x < 0 || new_x >= width) return MOVED_WALL; //move is not valid since out of map

    if (map[new_y * width + new_x] == WALL) return MOVED_WALL;//check if it wall
    map[*y * width + *x] = dot_map[*y * width + *x]; //replace ghost w map value (either EMPTY or DOT)
    map[new_y * width + new_x] = GHOST;//moved ghost on map

    //update ghost y,x
    *y = new_y;
    *x = new_x;
    return MOVED_OKAY;//move valid
}