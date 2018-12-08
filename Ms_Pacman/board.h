#ifndef BOARD_H
#define BOARD_H

#include "gl.h"

#define BOARD_WIDTH 224
#define BOARD_HEIGHT 288
#define TILE_WIDTH BOARD_WIDTH / 28
#define HALF_TILE TILE_WIDTH / 2

#define NUMBER_OF_DOTS 244
#define NUM_SUPER_DOTS 4

int board_get_tile_width();

void board_init(void);

void draw_dots(void);

void display_score(const char *format, ...);

int numDots;

color_t board_color;
color_t ghost_door;
color_t dot_color;
color_t super_dot_color;

#endif
