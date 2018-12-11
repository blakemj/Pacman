#ifndef BOARD_H
#define BOARD_H

#include "gl.h"

//Thsese define the board width and height, and how the grid is split up
#define BOARD_WIDTH 224
#define BOARD_HEIGHT 288
#define TILE_WIDTH BOARD_WIDTH / 28
#define HALF_TILE TILE_WIDTH / 2

//These define the number of dots and number of super dots on the board
#define NUMBER_OF_DOTS 244
#define NUM_SUPER_DOTS 4

//This gets the tile width
int board_get_tile_width();

//This initializes the board
void board_init(void);

//This draws the dots on the screen
void draw_dots(void);

//This keeps track of the number of dots left
int numDots;

//These variables represent the board color, the color of the ghost door, the dot color, and the super dot color
color_t board_color;
color_t ghost_door;
color_t dot_color;
color_t super_dot_color;

#endif
