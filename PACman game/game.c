// APSC 142 Engineering Programming Project Starter Code
// Copyright Sean Kauffman 2024

#include "defines.h"
#include "game.h"

//decalre some stuff
extern char * map, * dot_map; //poniter to dot and apsc142main
extern int height;
extern int width;

//fuction check win for player to win the game
//keep going until win (no more dot)

int check_win(void) {
    //loop thru all to check if any dots that r uneaten yet (including the spot's the ghosts r sitting on)
    for (int i = 0; i < height * width; i++) {
        if (dot_map[i] == DOT) {
            return KEEP_GOING;  // there are still dots to eat
        }
    }
    return YOU_WIN;  // all dots eaten
}

//function if player loss
int check_loss(int player_y, int player_x, int ghosts_y[NUM_GHOSTS], int ghosts_x[NUM_GHOSTS]) {
    //loop all ghost if it eat player, aka same spot
    for (int i = 0; i < NUM_GHOSTS; i++) {
        if (player_y == ghosts_y[i] && player_x == ghosts_x[i]) {//if either ghost and player in the same spot, then lose
            return YOU_LOSE; //ghost eat player
        }
    }
    return KEEP_GOING; // player is safe

}