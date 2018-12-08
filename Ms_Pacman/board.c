#include "gl.h"
#include "board.h"
#include "gl_more.h"

//
color_t board_color = GL_BLUE;
color_t ghost_door = GL_GREEN;
color_t dot_color = GL_WHITE;
color_t super_dot_color = GL_OFFWHITE;

//
int numDots;

/*
*
*/
int board_get_tile_width() {
    return TILE_WIDTH;
}

/*
* This function initializes the board. READ LINES OF CODE AT YOUR OWN RISK (just kidding, its just a lot of
* strategically drawn lines). This is the beginning/introduction to the game being split into tiles, as the
* the lines are drawn according to the tiles (or in the middle of the tiles to be more accurate). By splitting
* into tiles, the board is a much more managable grid that can be followed. The funciton starts by clearing the
* screen, then it draws the boarder, and then the shapes in order from top to bottom, left to right. 
*/
void board_init() {
    gl_clear(GL_BLACK);
    //This is the surrounding border
    gl_draw_line(HALF_TILE, 3*TILE_WIDTH + HALF_TILE, 13*8 + HALF_TILE, 3*8 + HALF_TILE, board_color);
    gl_draw_line(14*TILE_WIDTH + HALF_TILE, 3*TILE_WIDTH + HALF_TILE, 27*TILE_WIDTH + HALF_TILE, 3*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(14*TILE_WIDTH + HALF_TILE, 3*TILE_WIDTH + HALF_TILE, 14*TILE_WIDTH + HALF_TILE, 7*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(13*TILE_WIDTH + HALF_TILE, 3*TILE_WIDTH + HALF_TILE, 13*TILE_WIDTH + HALF_TILE, 7*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(13*TILE_WIDTH + HALF_TILE, 7*TILE_WIDTH + HALF_TILE, 14*TILE_WIDTH + HALF_TILE, 7*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(HALF_TILE, 3*TILE_WIDTH + HALF_TILE, HALF_TILE, 12*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(27*TILE_WIDTH + HALF_TILE, 3*TILE_WIDTH + HALF_TILE, 27*TILE_WIDTH + HALF_TILE, 12*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(HALF_TILE, 12*TILE_WIDTH + HALF_TILE, 5*TILE_WIDTH + HALF_TILE, 12*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(22*TILE_WIDTH + HALF_TILE, 12*TILE_WIDTH + HALF_TILE, 27*TILE_WIDTH + HALF_TILE, 12*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(5*TILE_WIDTH + HALF_TILE, 12*TILE_WIDTH + HALF_TILE, 5*TILE_WIDTH + HALF_TILE, 16*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(22*TILE_WIDTH + HALF_TILE, 12*TILE_WIDTH + HALF_TILE, 22*TILE_WIDTH + HALF_TILE, 16*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(0, 16*TILE_WIDTH + HALF_TILE, 5*TILE_WIDTH + HALF_TILE, 16*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(22*TILE_WIDTH + HALF_TILE, 16*TILE_WIDTH + HALF_TILE, gl_get_width(), 16*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(0, 18*TILE_WIDTH + HALF_TILE, 5*TILE_WIDTH + HALF_TILE, 18*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(22*TILE_WIDTH + HALF_TILE, 18*TILE_WIDTH + HALF_TILE, gl_get_width(), 18*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(5*TILE_WIDTH + HALF_TILE, 18*TILE_WIDTH + HALF_TILE, 5*TILE_WIDTH + HALF_TILE, 22*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(22*TILE_WIDTH + HALF_TILE, 18*TILE_WIDTH + HALF_TILE, 22*TILE_WIDTH + HALF_TILE, 22*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(HALF_TILE, 22*TILE_WIDTH + HALF_TILE, 5*TILE_WIDTH + HALF_TILE, 22*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(22*TILE_WIDTH + HALF_TILE, 22*TILE_WIDTH + HALF_TILE, 27*TILE_WIDTH + HALF_TILE, 22*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(HALF_TILE, 22*TILE_WIDTH + HALF_TILE, HALF_TILE, 27*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(27*TILE_WIDTH + HALF_TILE, 22*TILE_WIDTH + HALF_TILE, 27*TILE_WIDTH + HALF_TILE, 27*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(HALF_TILE, 27*TILE_WIDTH + HALF_TILE, 2*TILE_WIDTH + HALF_TILE, 27*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(25*TILE_WIDTH + HALF_TILE, 27*TILE_WIDTH + HALF_TILE, 27*TILE_WIDTH + HALF_TILE, 27*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(2*TILE_WIDTH + HALF_TILE, 27*TILE_WIDTH + HALF_TILE, 2*TILE_WIDTH + HALF_TILE, 28*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(25*TILE_WIDTH + HALF_TILE, 27*TILE_WIDTH + HALF_TILE, 25*TILE_WIDTH + HALF_TILE, 28*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(HALF_TILE, 28*TILE_WIDTH + HALF_TILE, 2*TILE_WIDTH + HALF_TILE, 28*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(25*TILE_WIDTH + HALF_TILE, 28*TILE_WIDTH + HALF_TILE, 27*TILE_WIDTH + HALF_TILE, 28*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(HALF_TILE, 28*TILE_WIDTH + HALF_TILE, HALF_TILE, 33*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(27*TILE_WIDTH + HALF_TILE, 28*TILE_WIDTH + HALF_TILE, 27*TILE_WIDTH + HALF_TILE, 33*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(HALF_TILE, 33*TILE_WIDTH + HALF_TILE, 27*TILE_WIDTH + HALF_TILE, 33*TILE_WIDTH + HALF_TILE, board_color);

    //These are the top 4 rectangles on the board (left to right)
    gl_draw_line(2*TILE_WIDTH + HALF_TILE, 5*TILE_WIDTH + HALF_TILE, 5*TILE_WIDTH + HALF_TILE, 5*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(2*TILE_WIDTH + HALF_TILE, 5*TILE_WIDTH + HALF_TILE, 2*TILE_WIDTH + HALF_TILE, 7*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(2*TILE_WIDTH + HALF_TILE, 7*TILE_WIDTH + HALF_TILE, 5*TILE_WIDTH + HALF_TILE, 7*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(5*TILE_WIDTH + HALF_TILE, 5*TILE_WIDTH + HALF_TILE, 5*TILE_WIDTH + HALF_TILE, 7*TILE_WIDTH + HALF_TILE, board_color);

    gl_draw_line(7*TILE_WIDTH + HALF_TILE, 5*TILE_WIDTH + HALF_TILE, 11*TILE_WIDTH + HALF_TILE, 5*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(7*TILE_WIDTH + HALF_TILE, 5*TILE_WIDTH + HALF_TILE, 7*TILE_WIDTH + HALF_TILE, 7*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(7*TILE_WIDTH + HALF_TILE, 7*TILE_WIDTH + HALF_TILE, 11*TILE_WIDTH + HALF_TILE, 7*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(11*TILE_WIDTH + HALF_TILE, 5*TILE_WIDTH + HALF_TILE, 11*TILE_WIDTH + HALF_TILE, 7*TILE_WIDTH + HALF_TILE, board_color);

    gl_draw_line(16*TILE_WIDTH + HALF_TILE, 5*TILE_WIDTH + HALF_TILE, 20*TILE_WIDTH + HALF_TILE, 5*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(16*TILE_WIDTH + HALF_TILE, 5*TILE_WIDTH + HALF_TILE, 16*TILE_WIDTH + HALF_TILE, 7*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(16*TILE_WIDTH + HALF_TILE, 7*TILE_WIDTH + HALF_TILE, 20*TILE_WIDTH + HALF_TILE, 7*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(20*TILE_WIDTH + HALF_TILE, 5*TILE_WIDTH + HALF_TILE, 20*TILE_WIDTH + HALF_TILE, 7*TILE_WIDTH + HALF_TILE, board_color);

    gl_draw_line(22*TILE_WIDTH + HALF_TILE, 5*TILE_WIDTH + HALF_TILE, 25*TILE_WIDTH + HALF_TILE, 5*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(22*TILE_WIDTH + HALF_TILE, 5*TILE_WIDTH + HALF_TILE, 22*TILE_WIDTH + HALF_TILE, 7*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(22*TILE_WIDTH + HALF_TILE, 7*TILE_WIDTH + HALF_TILE, 25*TILE_WIDTH + HALF_TILE, 7*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(25*TILE_WIDTH + HALF_TILE, 5*TILE_WIDTH + HALF_TILE, 25*TILE_WIDTH + HALF_TILE, 7*TILE_WIDTH + HALF_TILE, board_color);

    //These are the next 5 internal shapes (left to right)
    gl_draw_line(2*TILE_WIDTH + HALF_TILE, 9*TILE_WIDTH + HALF_TILE, 5*TILE_WIDTH + HALF_TILE, 9*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(2*TILE_WIDTH + HALF_TILE, 9*TILE_WIDTH + HALF_TILE, 2*TILE_WIDTH + HALF_TILE, 10*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(2*TILE_WIDTH + HALF_TILE, 10*TILE_WIDTH + HALF_TILE, 5*TILE_WIDTH + HALF_TILE, 10*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(5*TILE_WIDTH + HALF_TILE, 9*TILE_WIDTH + HALF_TILE, 5*TILE_WIDTH + HALF_TILE, 10*TILE_WIDTH + HALF_TILE, board_color);

    gl_draw_line(7*TILE_WIDTH + HALF_TILE, 9*TILE_WIDTH + HALF_TILE, 8*TILE_WIDTH + HALF_TILE, 9*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(7*TILE_WIDTH + HALF_TILE, 9*TILE_WIDTH + HALF_TILE, 7*TILE_WIDTH + HALF_TILE, 16*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(7*TILE_WIDTH + HALF_TILE, 16*TILE_WIDTH + HALF_TILE, 8*TILE_WIDTH + HALF_TILE, 16*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(8*TILE_WIDTH + HALF_TILE, 9*TILE_WIDTH + HALF_TILE, 8*TILE_WIDTH + HALF_TILE, 12*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(8*TILE_WIDTH + HALF_TILE, 13*TILE_WIDTH + HALF_TILE, 8*TILE_WIDTH + HALF_TILE, 16*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(8*TILE_WIDTH + HALF_TILE, 12*TILE_WIDTH + HALF_TILE, 11*TILE_WIDTH + HALF_TILE, 12*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(8*TILE_WIDTH + HALF_TILE, 13*TILE_WIDTH + HALF_TILE, 11*TILE_WIDTH + HALF_TILE, 13*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(11*TILE_WIDTH + HALF_TILE, 12*TILE_WIDTH + HALF_TILE, 11*TILE_WIDTH + HALF_TILE, 13*TILE_WIDTH + HALF_TILE, board_color);

    gl_draw_line(10*TILE_WIDTH + HALF_TILE, 9*TILE_WIDTH + HALF_TILE, 17*TILE_WIDTH + HALF_TILE, 9*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(10*TILE_WIDTH + HALF_TILE, 9*TILE_WIDTH + HALF_TILE, 10*TILE_WIDTH + HALF_TILE, 10*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(10*TILE_WIDTH + HALF_TILE, 10*TILE_WIDTH + HALF_TILE, 13*TILE_WIDTH + HALF_TILE, 10*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(14*TILE_WIDTH + HALF_TILE, 10*TILE_WIDTH + HALF_TILE, 17*TILE_WIDTH + HALF_TILE, 10*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(17*TILE_WIDTH + HALF_TILE, 9*TILE_WIDTH + HALF_TILE, 17*TILE_WIDTH + HALF_TILE, 10*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(13*TILE_WIDTH + HALF_TILE, 10*TILE_WIDTH + HALF_TILE, 13*TILE_WIDTH + HALF_TILE, 13*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(14*TILE_WIDTH + HALF_TILE, 10*TILE_WIDTH + HALF_TILE, 14*TILE_WIDTH + HALF_TILE, 13*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(13*TILE_WIDTH + HALF_TILE, 13*TILE_WIDTH + HALF_TILE, 14*TILE_WIDTH + HALF_TILE, 13*TILE_WIDTH + HALF_TILE, board_color);

    gl_draw_line(19*TILE_WIDTH + HALF_TILE, 9*TILE_WIDTH + HALF_TILE, 20*TILE_WIDTH + HALF_TILE, 9*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(20*TILE_WIDTH + HALF_TILE, 9*TILE_WIDTH + HALF_TILE, 20*TILE_WIDTH + HALF_TILE, 16*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(19*TILE_WIDTH + HALF_TILE, 16*TILE_WIDTH + HALF_TILE, 20*TILE_WIDTH + HALF_TILE, 16*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(19*TILE_WIDTH + HALF_TILE, 9*TILE_WIDTH + HALF_TILE, 19*TILE_WIDTH + HALF_TILE, 12*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(19*TILE_WIDTH + HALF_TILE, 13*TILE_WIDTH + HALF_TILE, 19*TILE_WIDTH + HALF_TILE, 16*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(16*TILE_WIDTH + HALF_TILE, 12*TILE_WIDTH + HALF_TILE, 19*TILE_WIDTH + HALF_TILE, 12*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(16*TILE_WIDTH + HALF_TILE, 13*TILE_WIDTH + HALF_TILE, 19*TILE_WIDTH + HALF_TILE, 13*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(16*TILE_WIDTH + HALF_TILE, 12*TILE_WIDTH + HALF_TILE, 16*TILE_WIDTH + HALF_TILE, 13*TILE_WIDTH + HALF_TILE, board_color);

    gl_draw_line(22*TILE_WIDTH + HALF_TILE, 9*TILE_WIDTH + HALF_TILE, 25*TILE_WIDTH + HALF_TILE, 9*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(22*TILE_WIDTH + HALF_TILE, 9*TILE_WIDTH + HALF_TILE, 22*TILE_WIDTH + HALF_TILE, 10*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(22*TILE_WIDTH + HALF_TILE, 10*TILE_WIDTH + HALF_TILE, 25*TILE_WIDTH + HALF_TILE, 10*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(25*TILE_WIDTH + HALF_TILE, 9*TILE_WIDTH + HALF_TILE, 25*TILE_WIDTH + HALF_TILE, 10*TILE_WIDTH + HALF_TILE, board_color);

    //This is the ghost prison box
    gl_draw_line(10*TILE_WIDTH + HALF_TILE, 15*TILE_WIDTH + HALF_TILE, 13*TILE_WIDTH, 15*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(15*TILE_WIDTH, 15*TILE_WIDTH + HALF_TILE, 17*TILE_WIDTH + HALF_TILE, 15*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(13*TILE_WIDTH, 15*TILE_WIDTH + HALF_TILE, 15*TILE_WIDTH, 15*TILE_WIDTH + HALF_TILE, ghost_door);
    gl_draw_line(10*TILE_WIDTH + HALF_TILE, 15*TILE_WIDTH + HALF_TILE, 10*TILE_WIDTH + HALF_TILE, 19*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(10*TILE_WIDTH + HALF_TILE, 19*TILE_WIDTH + HALF_TILE, 17*TILE_WIDTH + HALF_TILE, 19*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(17*TILE_WIDTH + HALF_TILE, 15*TILE_WIDTH + HALF_TILE, 17*TILE_WIDTH + HALF_TILE, 19*TILE_WIDTH + HALF_TILE, board_color);

    //These are the next three shapes (left to right)
    gl_draw_line(7*TILE_WIDTH + HALF_TILE, 18*TILE_WIDTH + HALF_TILE, 8*TILE_WIDTH + HALF_TILE, 18*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(7*TILE_WIDTH + HALF_TILE, 18*TILE_WIDTH + HALF_TILE, 7*TILE_WIDTH + HALF_TILE, 22*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(7*TILE_WIDTH + HALF_TILE, 22*TILE_WIDTH + HALF_TILE, 8*TILE_WIDTH + HALF_TILE, 22*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(8*TILE_WIDTH + HALF_TILE, 18*TILE_WIDTH + HALF_TILE, 8*TILE_WIDTH + HALF_TILE, 22*TILE_WIDTH + HALF_TILE, board_color);

    gl_draw_line(10*TILE_WIDTH + HALF_TILE, 21*TILE_WIDTH + HALF_TILE, 17*TILE_WIDTH + HALF_TILE, 21*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(10*TILE_WIDTH + HALF_TILE, 21*TILE_WIDTH + HALF_TILE, 10*TILE_WIDTH + HALF_TILE, 22*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(10*TILE_WIDTH + HALF_TILE, 22*TILE_WIDTH + HALF_TILE, 13*TILE_WIDTH + HALF_TILE, 22*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(14*TILE_WIDTH + HALF_TILE, 22*TILE_WIDTH + HALF_TILE, 17*TILE_WIDTH + HALF_TILE, 22*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(17*TILE_WIDTH + HALF_TILE, 21*TILE_WIDTH + HALF_TILE, 17*TILE_WIDTH + HALF_TILE, 22*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(13*TILE_WIDTH + HALF_TILE, 22*TILE_WIDTH + HALF_TILE, 13*TILE_WIDTH + HALF_TILE, 25*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(14*TILE_WIDTH + HALF_TILE, 22*TILE_WIDTH + HALF_TILE, 14*TILE_WIDTH + HALF_TILE, 25*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(13*TILE_WIDTH + HALF_TILE, 25*TILE_WIDTH + HALF_TILE, 14*TILE_WIDTH + HALF_TILE, 25*TILE_WIDTH + HALF_TILE, board_color);

    gl_draw_line(19*TILE_WIDTH + HALF_TILE, 18*TILE_WIDTH + HALF_TILE, 20*TILE_WIDTH + HALF_TILE, 18*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(19*TILE_WIDTH + HALF_TILE, 18*TILE_WIDTH + HALF_TILE, 19*TILE_WIDTH + HALF_TILE, 22*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(19*TILE_WIDTH + HALF_TILE, 22*TILE_WIDTH + HALF_TILE, 20*TILE_WIDTH + HALF_TILE, 22*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(20*TILE_WIDTH + HALF_TILE, 18*TILE_WIDTH + HALF_TILE, 20*TILE_WIDTH + HALF_TILE, 22*TILE_WIDTH + HALF_TILE, board_color);

    //These are the next 4 shapes
    gl_draw_line(2*TILE_WIDTH + HALF_TILE, 24*TILE_WIDTH + HALF_TILE, 5*TILE_WIDTH + HALF_TILE, 24*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(2*TILE_WIDTH + HALF_TILE, 24*TILE_WIDTH + HALF_TILE, 2*TILE_WIDTH + HALF_TILE, 25*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(2*TILE_WIDTH + HALF_TILE, 25*TILE_WIDTH + HALF_TILE, 4*TILE_WIDTH + HALF_TILE, 25*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(4*TILE_WIDTH + HALF_TILE, 25*TILE_WIDTH + HALF_TILE, 4*TILE_WIDTH + HALF_TILE, 28*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(4*TILE_WIDTH + HALF_TILE, 28*TILE_WIDTH + HALF_TILE, 5*TILE_WIDTH + HALF_TILE, 28*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(5*TILE_WIDTH + HALF_TILE, 24*TILE_WIDTH + HALF_TILE, 5*TILE_WIDTH + HALF_TILE, 28*TILE_WIDTH + HALF_TILE, board_color);

    gl_draw_line(7*TILE_WIDTH + HALF_TILE, 24*TILE_WIDTH + HALF_TILE, 11*TILE_WIDTH + HALF_TILE, 24*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(7*TILE_WIDTH + HALF_TILE, 24*TILE_WIDTH + HALF_TILE, 7*TILE_WIDTH + HALF_TILE, 25*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(7*TILE_WIDTH + HALF_TILE, 25*TILE_WIDTH + HALF_TILE, 11*TILE_WIDTH + HALF_TILE, 25*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(11*TILE_WIDTH + HALF_TILE, 24*TILE_WIDTH + HALF_TILE, 11*TILE_WIDTH + HALF_TILE, 25*TILE_WIDTH + HALF_TILE, board_color);

    gl_draw_line(16*TILE_WIDTH + HALF_TILE, 24*TILE_WIDTH + HALF_TILE, 20*TILE_WIDTH + HALF_TILE, 24*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(16*TILE_WIDTH + HALF_TILE, 24*TILE_WIDTH + HALF_TILE, 16*TILE_WIDTH + HALF_TILE, 25*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(16*TILE_WIDTH + HALF_TILE, 25*TILE_WIDTH + HALF_TILE, 20*TILE_WIDTH + HALF_TILE, 25*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(20*TILE_WIDTH + HALF_TILE, 24*TILE_WIDTH + HALF_TILE, 20*TILE_WIDTH + HALF_TILE, 25*TILE_WIDTH + HALF_TILE, board_color);

    gl_draw_line(22*TILE_WIDTH + HALF_TILE, 24*TILE_WIDTH + HALF_TILE, 25*TILE_WIDTH + HALF_TILE, 24*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(25*TILE_WIDTH + HALF_TILE, 24*TILE_WIDTH + HALF_TILE, 25*TILE_WIDTH + HALF_TILE, 25*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(22*TILE_WIDTH + HALF_TILE, 24*TILE_WIDTH + HALF_TILE, 22*TILE_WIDTH + HALF_TILE, 28*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(22*TILE_WIDTH + HALF_TILE, 28*TILE_WIDTH + HALF_TILE, 23*TILE_WIDTH + HALF_TILE, 28*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(23*TILE_WIDTH + HALF_TILE, 25*TILE_WIDTH + HALF_TILE, 23*TILE_WIDTH + HALF_TILE, 28*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(23*TILE_WIDTH + HALF_TILE, 25*TILE_WIDTH + HALF_TILE, 25*TILE_WIDTH + HALF_TILE, 25*TILE_WIDTH + HALF_TILE, board_color);

    //These are the final 3 shapes on the bottom (left to right)
    gl_draw_line(7*TILE_WIDTH + HALF_TILE, 27*TILE_WIDTH + HALF_TILE, 8*TILE_WIDTH + HALF_TILE, 27*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(7*TILE_WIDTH + HALF_TILE, 27*TILE_WIDTH + HALF_TILE, 7*TILE_WIDTH + HALF_TILE, 30*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(8*TILE_WIDTH + HALF_TILE, 27*TILE_WIDTH + HALF_TILE, 8*TILE_WIDTH + HALF_TILE, 30*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(2*TILE_WIDTH + HALF_TILE, 30*TILE_WIDTH + HALF_TILE, 7*TILE_WIDTH + HALF_TILE, 30*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(8*TILE_WIDTH + HALF_TILE, 30*TILE_WIDTH + HALF_TILE, 11*TILE_WIDTH + HALF_TILE, 30*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(2*TILE_WIDTH + HALF_TILE, 30*TILE_WIDTH + HALF_TILE, 2*TILE_WIDTH + HALF_TILE, 31*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(11*TILE_WIDTH + HALF_TILE, 30*TILE_WIDTH + HALF_TILE, 11*TILE_WIDTH + HALF_TILE, 31*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(2*TILE_WIDTH + HALF_TILE, 31*TILE_WIDTH + HALF_TILE, 11*TILE_WIDTH + HALF_TILE, 31*TILE_WIDTH + HALF_TILE, board_color);

    gl_draw_line(10*TILE_WIDTH + HALF_TILE, 27*TILE_WIDTH + HALF_TILE, 17*TILE_WIDTH + HALF_TILE, 27*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(10*TILE_WIDTH + HALF_TILE, 27*TILE_WIDTH + HALF_TILE, 10*TILE_WIDTH + HALF_TILE, 28*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(10*TILE_WIDTH + HALF_TILE, 28*TILE_WIDTH + HALF_TILE, 13*TILE_WIDTH + HALF_TILE, 28*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(14*TILE_WIDTH + HALF_TILE, 28*TILE_WIDTH + HALF_TILE, 17*TILE_WIDTH + HALF_TILE, 28*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(17*TILE_WIDTH + HALF_TILE, 27*TILE_WIDTH + HALF_TILE, 17*TILE_WIDTH + HALF_TILE, 28*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(13*TILE_WIDTH + HALF_TILE, 28*TILE_WIDTH + HALF_TILE, 13*TILE_WIDTH + HALF_TILE, 31*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(14*TILE_WIDTH + HALF_TILE, 28*TILE_WIDTH + HALF_TILE, 14*TILE_WIDTH + HALF_TILE, 31*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(13*TILE_WIDTH + HALF_TILE, 31*TILE_WIDTH + HALF_TILE, 14*TILE_WIDTH + HALF_TILE, 31*TILE_WIDTH + HALF_TILE, board_color);

    gl_draw_line(19*TILE_WIDTH + HALF_TILE, 27*TILE_WIDTH + HALF_TILE, 20*TILE_WIDTH + HALF_TILE, 27*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(19*TILE_WIDTH + HALF_TILE, 27*TILE_WIDTH + HALF_TILE, 19*TILE_WIDTH + HALF_TILE, 30*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(20*TILE_WIDTH + HALF_TILE, 27*TILE_WIDTH + HALF_TILE, 20*TILE_WIDTH + HALF_TILE, 30*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(16*TILE_WIDTH + HALF_TILE, 30*TILE_WIDTH + HALF_TILE, 19*TILE_WIDTH + HALF_TILE, 30*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(20*TILE_WIDTH + HALF_TILE, 30*TILE_WIDTH + HALF_TILE, 25*TILE_WIDTH + HALF_TILE, 30*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(16*TILE_WIDTH + HALF_TILE, 30*TILE_WIDTH + HALF_TILE, 16*TILE_WIDTH + HALF_TILE, 31*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(25*TILE_WIDTH + HALF_TILE, 30*TILE_WIDTH + HALF_TILE, 25*TILE_WIDTH + HALF_TILE, 31*TILE_WIDTH + HALF_TILE, board_color);
    gl_draw_line(16*TILE_WIDTH + HALF_TILE, 31*TILE_WIDTH + HALF_TILE, 25*TILE_WIDTH + HALF_TILE, 31*TILE_WIDTH + HALF_TILE, board_color);
}

/*
*
*/
void draw_dots() {
    //This draws the dots in the first half of the screen (minus the second row due to super dots)
    for (int i = HALF_TILE; i < BOARD_WIDTH; i = i + TILE_WIDTH) {
        for (int j = 4*TILE_WIDTH + HALF_TILE; j < 12*TILE_WIDTH + HALF_TILE; j = j + TILE_WIDTH) {
            if (j != 6*TILE_WIDTH + HALF_TILE && gl_read_pixel(i, j) != board_color) {
                gl_draw_pixel(i, j, dot_color);
            }
        } 
    }
    //This draws the two columns of dots in the middle of the screen
    for (int j = 4*TILE_WIDTH + HALF_TILE; j < 33*TILE_WIDTH + HALF_TILE; j = j + TILE_WIDTH) {
        if (gl_read_pixel(6*TILE_WIDTH + HALF_TILE, j) != board_color && gl_read_pixel(6*TILE_WIDTH + HALF_TILE, j) != dot_color) {
            gl_draw_pixel(6*TILE_WIDTH + HALF_TILE, j, dot_color);
        }
        if (gl_read_pixel(21*TILE_WIDTH + HALF_TILE, j) != board_color && gl_read_pixel(21*TILE_WIDTH + HALF_TILE, j) != dot_color) {
            gl_draw_pixel(21*TILE_WIDTH + HALF_TILE, j, dot_color);
        }
    } 
    //This draws the dots in the bottom half of the screen
    for (int i = HALF_TILE; i < BOARD_WIDTH; i = i + TILE_WIDTH) {
        for (int j = 23*TILE_WIDTH + HALF_TILE; j < 33*TILE_WIDTH + HALF_TILE; j = j + TILE_WIDTH) {
            if (!(j == 26*TILE_WIDTH + HALF_TILE && (i == TILE_WIDTH + HALF_TILE || i == 26*TILE_WIDTH + HALF_TILE)) && gl_read_pixel(i, j) != board_color) {
                gl_draw_pixel(i, j, dot_color);
            }
        }
    }
    //These are the two dots that are skipped in the second row
    gl_draw_pixel(12*TILE_WIDTH + HALF_TILE, 6*TILE_WIDTH + HALF_TILE, dot_color);
    gl_draw_pixel(15*TILE_WIDTH + HALF_TILE, 6*TILE_WIDTH + HALF_TILE, dot_color);
    //This draws the super dots
    gl_draw_circle(TILE_WIDTH, 6*TILE_WIDTH, TILE_WIDTH, TILE_WIDTH, super_dot_color);
    gl_draw_circle(26*TILE_WIDTH, 6*TILE_WIDTH, TILE_WIDTH, TILE_WIDTH, super_dot_color);
    gl_draw_circle(TILE_WIDTH, 26*TILE_WIDTH, TILE_WIDTH, TILE_WIDTH, super_dot_color);
    gl_draw_circle(26*TILE_WIDTH, 26*TILE_WIDTH, TILE_WIDTH, TILE_WIDTH, super_dot_color);
    numDots = NUMBER_OF_DOTS;
}

