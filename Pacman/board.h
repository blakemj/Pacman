#ifndef BOARD_H
#define BOARD_H

void board_init(void);

void draw_dots(void);

void display_score(const char *format, ...);

int numDots;

#endif
