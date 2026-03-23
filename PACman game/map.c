// APSC 142 Engineering Programming Project Starter Code
// Copyright Sean Kauffman 2024

#include <stdio.h>

#include "defines.h"
#include "colours.h"
#include "map.h"

#include <stdlib.h>

extern char *map, *dot_map;
extern int width, height;

/**
 * You should use this function to print out individual characters
 * in the colours used in the demo.
 * @param c
 */

// print in right colour for wall ghost and player and dot
static void printc(char c) {
    switch (c) {
        case WALL:
            change_text_colour(BLUE);
            break;
        case GHOST:
            change_text_colour(PINK);
            break;
        case PLAYER:
            change_text_colour(YELLOW);
            break;
        case DOT:
            change_text_colour(WHITE);
            break;
        default:
            change_text_colour(WHITE); //everthing except yhose are white
    }
    printf("%c", c); //print those char
}
//print the map and wall outside
void print_map(void) {
    //top wall print
    for (int i = 0; i < width + 2; i++) {
        printc(WALL);
        printf(" ");
    }
    printf("\n");

    //go all row for actual map
    for (int i = 0; i < height; i++) {
        printc(WALL);
        printf(" ");// prints left wall
        for (int j = 0; j < width; j++) {//prints the stuff inbetween
            printc(map[i * width + j]);
            printf(" ");
        }
        printc(WALL);
        printf("\n"); //prints right wall
    }
    //prints bottom wall
    for (int i = 0; i < width + 2; i++) {
        printc(WALL);
        printf(" ");
    }
    printf("\n");
}

int is_wall(int y, int x) {
    //check if spot even inside
    if (y < 0 || y >= height || x < 0 || x >= width) {
        return 1;  //if not, it is a wall
    }

    // Check map if it wall
    if (map[y * width + x] == WALL) {
        return 1; //wall found
    }
    return 0;  //it not wall
}



char * load_map(char * fileName, int *mapHeight, int *mapWidth) {
    FILE *filepointer=NULL; //point to the flie
    filepointer=fopen(fileName, "r"); //open it
    if (filepointer==NULL) {
        printf("No File Found\n"); //cant find it
        return NULL; //final return
    }
    // satart off 1by1 to find the map size
    *mapHeight=1;
    *mapWidth=1;

    //declare map variables and set as 0 for now
    int widthNumber=0;
    int heightNumber=0;
    char hold; //temp holder at this time, declare outside of loop

    //read the first line to fin dhow long the map is
    while (!feof(filepointer)) {
        hold=getc(filepointer);
        if (hold=='\n') {
            heightNumber++;// the end of y-axis
            break;
        }

        widthNumber++;//count how many charater be for it \n
    }

    *mapWidth=(widthNumber+2)/3; // for spaces between them
    // keep read for how big like x-axis
    while (!feof(filepointer)) {
        if (getc(filepointer)=='\n') {
            heightNumber++;
        }
    }

    *mapHeight=heightNumber+1;// add one cuz we alrready read 1

    fclose(filepointer); // close it we done with the countign

    //alloate for actual map
    map=malloc((*mapHeight)*(*mapWidth));
    filepointer=fopen(fileName,"r"); //reopen file

    if (filepointer==NULL) {
        printf("No File Found\n"); //if the fail ._. somehing wrong.. >//<
        return NULL;
    }

    // go row by row in map
    for (int i=0; i<*mapHeight; i++) {
        for (int j=0; j<*mapWidth; j++) {
            hold=getc(filepointer);//grab next map
            map[i*(*mapWidth)+j]=hold;//save it to map array

            if (j<*mapWidth-1) {
                getc(filepointer);//skip spac
                getc(filepointer);//skip second space
            }

        }
        getc(filepointer);//skip newline at the end of row
    }

    fclose(filepointer);//done so we close the flie

    return map;//back to load map
}