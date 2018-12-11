#ifndef PACMAN_CHAR_H
#define PACMAN_CHAR_H

#include "gl.h"

//These initialize the two different pacmans for the game
void pacman_init();
void ms_pacman_init();

//This function will draw a black rectangle at the given x,y to erase either pacman
void erase_pacman(int x, int y);

//This funciton is used to draw the regular pacman
void draw_pacman(int x, int y, unsigned char move);

//This function will move and update the regular pacman
void pacman_move();

//THis function will move and update Ms. Pacman
void ms_pacman_move();

//This function will determine whether the side of a pacman at the x, y going in a direction will hit the given color
int check_sides(int x, int y, unsigned char direction, color_t color);

//This will return the x, y of pacman
int pacman_get_x();
int pacman_get_y();

//This will return whether pacman hit a ghost
int pacman_hit_ghost();

//This will return the x, y of Ms. Pacman
int ms_pacman_get_x();
int ms_pacman_get_y();

//This will return whether Ms. Pacman has hit a ghost
int ms_pacman_hit_ghost();

//This will return the current movement directions of the pacman
unsigned char pacman_get_curMove();
unsigned char ms_pacman_get_curMove();

//These variables will keep track of the extra points given for the super dot, and for frightened mode
int superDotBonus;
int frightened_points;
int frightenedPointsNotAdded;

#endif
