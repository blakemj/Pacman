#ifndef PACMAN_CHAR_H
#define PACMAN_CHAR_H

#include "gl.h"

void pacman_init();

int numDots;

void pacman_move();

int check_sides(int x, int y, unsigned char direction, color_t color);

int pacman_get_x();

int pacman_get_y();

#endif
