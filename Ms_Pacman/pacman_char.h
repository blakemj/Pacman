#ifndef PACMAN_CHAR_H
#define PACMAN_CHAR_H

#include "gl.h"

void pacman_init();

void ms_pacman_init();

void erase_pacman(int x, int y);

void draw_pacman(int x, int y, unsigned char move);

void pacman_move();

void ms_pacman_move();

int check_sides(int x, int y, unsigned char direction, color_t color);

int pacman_get_x();

int pacman_get_y();

int pacman_hit_ghost();

int ms_pacman_get_x();

int ms_pacman_get_y();

int ms_pacman_hit_ghost();

unsigned char pacman_get_curMove();

unsigned char ms_pacman_get_curMove();

int superDotBonus;
int frightened_points;
int frightenedPointsNotAdded;

#endif
