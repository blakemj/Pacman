#ifndef GHOSTS_H
#define GHOSTS_H

//
#define BLINKY_SCARED GL_PURPLE1
#define BLINKY_SCARED_ALT GL_HOTPINK1
#define PINKY_SCARED GL_PURPLE2
#define PINKY_SCARED_ALT GL_HOTPINK2
#define INKY_SCARED GL_PURPLE3
#define INKY_SCARED_ALT GL_HOTPINK3
#define CLYDE_SCARED GL_PURPLE4
#define CLYDE_SCARED_ALT GL_HOTPINK4

//
#define BLINKY_REG_COLOR GL_RED
#define PINKY_REG_COLOR GL_MAGENTA
#define INKY_REG_COLOR GL_CYAN
#define CLYDE_REG_COLOR GL_AMBER

void ghosts_init();

void blinky_move();

void erase_blinky();

void pinky_move();

void erase_pinky();

void inky_move();

void erase_inky();

void clyde_move();

void erase_clyde();

int frightened;
int frightened_start;

int blinky_caught;
int pinky_caught;
int inky_caught;
int clyde_caught;
int blinky_to_center;
int pinky_to_center;
int inky_to_center;
int clyde_to_center;

#endif
