#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <stdio.h>

// make sure not to modify anything in this extern block
extern "C"{
#include "defines.h"
#include "map.h"
#include "actor.h"
#include "game.h"
char *map = NULL, *dot_map = NULL;
int width, height;
}
#define NUM_GHOSTS 2
/**
 * This file is where you should put your tests for your code.
 * Your code must have tests that execute at least 85% of the code in
 * required functions for you to get full marks for the project.
 * Make sure to check out the course videos on automated testing for
 * more information about how to write tests.
 */

/* tests for map.c */
TEST_SUITE_BEGIN("Map tests"); //for all map test

TEST_CASE("is_wall") {
    int height = 11, width = 11;  // Map size
    char *test_map = load_map("map.txt", &height, &width);  // Load the map from file
    REQUIRE(test_map != NULL);  // make sure if map loaded

    map = test_map;  // set global map so other funtion can use it

    // test if that 00 10 10 is wall
    CHECK(is_wall(0, 0) == 1);  // edge wall)
    CHECK(is_wall(10, 10) == 1);  // another edge wall)

    // inside diff position map
    CHECK(is_wall(4, 4) == 0);  // not wall cuz it palyer spot
    CHECK(is_wall(2, 2) == 0);  // just emty space ;)
    CHECK(is_wall(1, 1) == 1);  // 11 is wall!!! ._.

    // try outside map
    CHECK(is_wall(-1, 0) == 1);
    CHECK(is_wall(11, 0) == 1);
    CHECK(is_wall(0, -1) == 1);
    CHECK(is_wall(0, 11) == 1);

    free(test_map);  // clear memory yay!!
}

TEST_CASE("Test for load_map") {
    int height = 10, width = 10;  // 10x10 map
    char *test_map = load_map("map.txt", &height, &width);

    REQUIRE(test_map != NULL);  // it work?

    // makesure height and width are set correctly
    CHECK(height == 10);
    CHECK(width == 10);

    // Now we check if specific characters are in the right place
    CHECK(test_map[1 * width + 1] == 'G');  // ghost spot
    CHECK(test_map[4 * width + 4] == 'P');  // player spot
    CHECK(test_map[2 * width + 2] == '.');  // dot spot

    // Check if the outermost edges are walls
    CHECK(test_map[0] == 'W');             // Top-left corner
    CHECK(test_map[width - 1] == 'W');     // Top-right corner
    CHECK(test_map[(height - 1) * width] == 'W'); // Bottom-left corner
    CHECK(test_map[(height - 1) * width + (width - 1)] == 'W');  // Bottom-right corner


    free(test_map); //done!
}

TEST_SUITE_END();

/* tests for actor.c */
TEST_SUITE_BEGIN("Actor tests");


TEST_CASE("Test move_ghost") {
    int ghost_y = 0;
    int ghost_x = 0;

    // 2x2 test map
    // GHOST DOT
    // DOT   WALL
    char tiny_map[] = {
            GHOST, DOT,
            DOT, WALL
    };

    height = 2;
    width = 2;
    map = tiny_map;

    char tiny_dot_map[] = {
            EMPTY, DOT,
            DOT, WALL
    };
    dot_map = tiny_dot_map;

    // defien direction
    char directionRight = RIGHT;
    char directionLeft = LEFT;
    char directionUp = UP;
    char directionDown = DOWN;
    char directionInvalid = 't'; // nO no u cant do that!

    // Move ghost riht in to dot
    CHECK(move_ghost(&ghost_y, &ghost_x, directionRight) == MOVED_OKAY);
    CHECK(ghost_y == 0);
    CHECK(ghost_x == 1);

    // Move left back to previous position (valid)
    CHECK(move_ghost(&ghost_y, &ghost_x, directionLeft) == MOVED_OKAY);
    CHECK(ghost_y == 0);
    CHECK(ghost_x == 0);

    // Move down to DOT (valid)
    CHECK(move_ghost(&ghost_y, &ghost_x, directionDown) == MOVED_OKAY);
    CHECK(ghost_y == 1);
    CHECK(ghost_x == 0);

    // Move right into WALL (invalid)
    CHECK(move_ghost(&ghost_y, &ghost_x, directionRight) == MOVED_WALL);
    CHECK(ghost_y == 1);
    CHECK(ghost_x == 0);

    // Move up back to original position (valid)
    CHECK(move_ghost(&ghost_y, &ghost_x, directionUp) == MOVED_OKAY);
    CHECK(ghost_y == 0);
    CHECK(ghost_x == 0);

    // Try invalid direction
    CHECK(move_ghost(&ghost_y, &ghost_x, directionInvalid) == MOVED_INVALID_DIRECTION);
}



TEST_CASE("Test move_player") {
    int player_y = 0, player_x = 0;

    char tiny_map[] = {
            PLAYER, DOT,
            DOT, WALL
    };
    height = 2;
    width = 2;
    map = tiny_map;

    char tiny_dot_map[] = {
            EMPTY, DOT,
            DOT, WALL
    };
    dot_map = tiny_dot_map;

    // try to more every possible ways -some ok but some are ""NO NO >,<
    CHECK(move_player(&player_y, &player_x, RIGHT) == MOVED_OKAY);
    CHECK(move_player(&player_y, &player_x, LEFT) == MOVED_OKAY);
    CHECK(move_player(&player_y, &player_x, DOWN) == MOVED_OKAY);
    CHECK(move_player(&player_y, &player_x, RIGHT) == MOVED_WALL);// hit wall
    CHECK(move_player(&player_y, &player_x, UP) == MOVED_OKAY);
    CHECK(move_player(&player_y, &player_x, 't') == MOVED_INVALID_DIRECTION);// you cant do it sorry broski
}

TEST_SUITE_END(); // done yay!


/* tests for game.c */
TEST_SUITE_BEGIN("Game tests");

TEST_CASE("Test check_win") {
    width = 3;
    height = 3;
    char test_dot_map[9];

    SUBCASE("Game continues when dots are present") {
        memset(test_dot_map, EMPTY, sizeof(test_dot_map)); //fill with no dots
        test_dot_map[4] = DOT;//add 1 dot
        dot_map = test_dot_map;
        CHECK(check_win() == KEEP_GOING); // cant win yet
    }

    SUBCASE("Game ends when no dots remain") {
        memset(test_dot_map, EMPTY, sizeof(test_dot_map)); //no more dot
        dot_map = test_dot_map;
        CHECK(check_win() == YOU_WIN);// now you can win
    }
}

TEST_CASE("Test for check_loss") {
    int player_y = 5;
    int player_x = 5;
    int ghosts_y[NUM_GHOSTS] = {4, 9};
    int ghosts_x[NUM_GHOSTS] = {4, 9};

    //player not close to ghost kepp playing
    CHECK(check_loss(player_y, player_x, ghosts_y, ghosts_x) == KEEP_GOING);

    //ghost catch!! noooooooo
    player_y = 4; player_x = 4;
    ghosts_y[0] = 4; ghosts_x[0] = 4;
    CHECK(check_loss(player_y, player_x, ghosts_y, ghosts_x) == YOU_LOSE);

    //same with second ghost
    player_y = 9; player_x = 9;
    ghosts_y[0] = 9; ghosts_x[0] = 9;
    CHECK(check_loss(player_y, player_x, ghosts_y, ghosts_x) == YOU_LOSE);

    // still far ,keep play
    player_y = 1; player_x = 1;
    ghosts_y[0] = 4; ghosts_x[0] = 4;
    CHECK(check_loss(player_y, player_x, ghosts_y, ghosts_x) == KEEP_GOING);

    //another match ;lost
    player_y = 0; player_x = 0;
    ghosts_y[0] = 0; ghosts_x[0] = 0;  // set ghost to match player position
    CHECK(check_loss(player_y, player_x, ghosts_y, ghosts_x) == YOU_LOSE);
}

TEST_CASE("Tests if sees_player handles eating player") {
    CHECK(sees_player(5, 5, 5, 5) == EATING_PLAYER);
}

TEST_CASE("Tests if sees_player handles horizontal vision with no walls") {
    width = 10;
    height = 10;
    char temp_map[100];
    map = temp_map;

    std::fill_n(map, 100, EMPTY);//clear map
    map[4 * width + 2] = GHOST;
    map[4 * width + 7] = PLAYER;

    // wee player R L with no walls
    CHECK(sees_player(4, 7, 4, 2) == RIGHT);
    CHECK(sees_player(4, 2, 4, 7) == LEFT);
}

TEST_CASE("Tests if sees_player handles vertical vision with no walls") {
    width = 10;
    height = 10;
    char temp_map[100];
    map = temp_map;

    std::fill_n(map, 100, EMPTY);
    map[2 * width + 4] = GHOST;
    map[7 * width + 4] = PLAYER;

    //ghost see player
    CHECK(sees_player(7, 4, 2, 4) == DOWN);
    CHECK(sees_player(2, 4, 7, 4) == UP);
}

TEST_CASE("Tests if sees_player is blocked by walls") {
    //set test variables
    width = 10;
    height = 10;
    char temp_map[100];
    map = temp_map;

    std::fill_n(map, 100, EMPTY);
    map[5 * width + 3] = WALL; //wall between

    CHECK(sees_player(5, 5, 5, 1) == SEES_NOTHING);
    CHECK(sees_player(5, 1, 5, 5) == SEES_NOTHING);
}

TEST_CASE("Tests if sees_player returns SEES_NOTHING for diagonal positions") {
    //ghost cant see in cross
    CHECK(sees_player(2, 3, 1, 1) == SEES_NOTHING);
}

TEST_SUITE_END(); //finallly we done, ngl this class tuff i submit liek 70+ times with tear
//lmao, this projects due date was poorly planned imo, needa study for exams now