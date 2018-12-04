#ifndef PACMAN_CHAR_H
#define PACMAN_CHAR_H

#include "gl.h"

void pacman_init();

void erase_pacman();

void draw_pacman(int x, int y);

void pacman_move();

int check_sides(int x, int y, unsigned char direction, color_t color);

int pacman_get_x();

int pacman_get_y();

int pacman_hit_ghost();

unsigned char pacman_get_curMove();

int superDotBonus;
int frightened_points;
int frightenedPointsNotAdded;

#endif
