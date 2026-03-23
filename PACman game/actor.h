// APSC 142 Engineering Programming Project Starter Code
// Copyright Sean Kauffman 2024

#ifndef PROJECT_ACTOR_H
#define PROJECT_ACTOR_H

/** Functions required to be implemented **/

#define SEES_NOTHING 0
#define EATING_PLAYER 1
/**
 * Checks if a ghost can see player, and in what direction they must
 * look to see him.
 * This function should use the passed coordinates of player and one
 * ghost to see if the ghost can see player by looking in a straight
 * line without looking through a wall.  The function can assume that
 * the global map / height / width are valid.
 *
 * If the ghost can see player without looking through a wall, the
 * function should return a direction that the ghost must look to see
 * him (LEFT, RIGHT, UP, DOWN) as defined in defines.h.  If the ghost
 * is eating player, the function should return EATING_PLAYER.  If
 * the ghost cannot see player, the function should return SEES_NOTHING.
 * @param player_y Player's Y coordinate
 * @param player_x Player's X coordinate
 * @param ghost_y The ghost's Y coordinate
 * @param ghost_x The ghost's X coordinate
 * @return The direction the ghost must look to see player, if possible.
 *         Otherwise, a status code.
 */
char sees_player(int player_y, int player_x, int ghost_y, int ghost_x);

#define MOVED_OKAY 0
#define MOVED_WALL 1
#define MOVED_INVALID_DIRECTION 2

/**
 * Try to move the player at the given coordinates, respecting walls and
 * updating the dot_map to remove a dot if required.
 * This function is passed the coordinates of the player.  The function
 * also takes a direction that the player should move (UP, DOWN, LEFT,
 * RIGHT) defined in defines.h.
 * The coordinates are passed by reference so that they can be updated
 * by the function, while the return of the function is a status code.
 *
 * If the move is valid, the function must update map to move the player.
 * The old location of the player must be replaced by EMPTY and the new
 * location must be updated to hold PLAYER.
 * Also, if the move is valid, the function must update dot_map to remove
 * any DOT at the location where the player has moved and replace it with
 * EMPTY.
 *
 * If the player cannot move because it would run into a wall, the
 * function should return MOVED_WALL and not update the map, dot_map or the
 * coordinate pointers.  If the direction that was passed was not a
 * valid direction, the function should return MOVED_INVALID_DIRECTION.
 * If the function successfully moves the player, it should return
 * MOVED_OKAY.
 *
 * Preconditions: y and x must not be NULL
 * Postconditions: If the move is valid, y and x will be updated with
 *   the new coordinates, map and dot_map will be updated to remove dots
 *   at the old location, move the player, and replace its old location
 *   with EMPTY.  Returns MOVED_OKAY.
 *   If the move is invalid, none of those updates will occur and the the
 *   function will return the appropriate status code: MOVED_WALL or
 *   MOVED_INVALID_DIRECTION.
 *
 * @param y A pointer to the Y position of the actor to move
 * @param x A pointer to the X position of the actor to move
 * @param direction The direction to try to move
 * @return A status code depending on the result of the function
 */
int move_player(int * y, int * x, char direction);

/**
 * Try to move a ghost at the given coordinates, respecting walls and
 * updating the map to replace a dot if required.
 * This function is passed the coordinates of a ghost.  The function
 * also takes a direction that the ghost should move (UP, DOWN, LEFT,
 * RIGHT) defined in defines.h.
 * The coordinates are passed by reference so that they can be updated
 * by the function, while the return of the function is a status code.
 *
 * The function must also update the global map to replace dots that were
 * moved over.  If the ghost moves off a location that contains a DOT in
 * the dot_map, the vacated coordinates in map must be replaced by DOT.
 * In either case, the global map should be updated at the new coordinates,
 * if moving was successful, to contain the ghost.
 *
 * If the ghost cannot move because it would run into a wall, the
 * function should return MOVED_WALL and not update the map or the
 * coordinate pointers.  If the direction that was passed was not a
 * valid direction, the function should return MOVED_INVALID_DIRECTION.
 * If the function successfully moves the ghost, it should return
 * MOVED_OKAY.
 *
 * Preconditions: y and x must not be NULL
 * Postconditions: If the move is valid, y and x will be updated with
 *   the new coordinates, map will be updated to replace dots
 *   at the old location (or replace with EMPTY) and move the ghost.
 *   Returns MOVED_OKAY.
 *   If the move is invalid, none of those updates will occur and the the
 *   function will return the appropriate status code: MOVED_WALL or
 *   MOVED_INVALID_DIRECTION.
 *
 * @param y A pointer to the Y position of the actor to move
 * @param x A pointer to the X position of the actor to move
 * @param direction The direction to try to move
 * @return A status code depending on the result of the function
 */
int move_ghost(int * y, int * x, char direction);

/** Other function prototypes can go below here **/


#endif //PROJECT_ACTOR_H
