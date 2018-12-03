#ifndef GHOSTS_H
#define GHOSTS_H

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
