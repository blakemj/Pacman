#ifndef BOARD_H
#define BOARD_H

#include "gl.h"

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
