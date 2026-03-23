// APSC 142 Engineering Programming Project Starter Code
// Copyright Sean Kauffman 2024

#ifndef PROJECT_MAP_H
#define PROJECT_MAP_H

/** Functions required to be implemented **/

/**
 * Prints the map using the global variables defined in apsc142project.c.
 * Preconditions: the map, dot_map, width, and height globals can be
 * assumed to be valid.
 */
void print_map(void);


/**
 * Loads a map file into an array allocated dynamically to store it.
 * The function must allocate space using malloc/realloc and load
 * the map file contents into it.  The return value is the pointer
 * to the allocated and populated memory, and the height/width values
 * that are returned via the map_height/map_width pass-by-reference
 * parameters.
 *
 * The function can assume that the parameters are valid pointers, but
 * it should not assume that the file exists.  The map file format
 * is #__#__# ... __# where the # are symbols and _ represent whitespace
 * which should be ignored.  That is, the function can assume that
 * symbols are each separated by two spaces.  The function MAY NOT
 * assume that the map is a specific size; it must properly load any
 * size map in the correct format.  It can assume that the only valid
 * symbols are DOT, PLAYER, GHOST, WALL, and EMPTY as defined in
 * defines.h.  The function can assume that the map is a rectangle.
 *
 * On error, the function should return NULL and should not write to
 * map_height or map_width.
 *
 * For example, if the map file contains the map:
 * .  P  .  .
 * W     W  W
 * G  .  .  G
 * Then the function should return a dynamically allocated array:
 * {'.', 'P', '.', '.', 'W', ' ', 'W', 'W', 'G', '.', '.', 'G'}
 * and set *map_height = 3, *map_width = 4.
 *
 * @param filename A string containing the map file name to read
 * @param map_height A pointer to where the map height should be stored
 * @param map_width A pointer to where the map width should be stored
 * @return A pointer to the loaded map, or NULL on error
 */
char * load_map(char * filename, int * map_height, int *map_width);

#define NOT_WALL 0
#define YES_WALL 1
/**
 * Determines if a coordinate in the map is a wall or not.
 * This function should use the global map and the passed coordinates
 * to determine a yes/no answer if this coordinate is a wall.
 *
 * You should use the above #defines for return values from
 * this function.  This function can assume that the global map
 * and its height/width are all valid.
 *
 * @param y The Y coordinate on the map to check
 * @param x The X coordinate on the map to check
 * @return NOT_WALL if the coordinate is not a wall, YES_WALL if it is
 */
int is_wall(int y, int x);



/** Other function prototypes can go below here **/


#endif //PROJECT_MAP_H
