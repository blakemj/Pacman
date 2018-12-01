#include "gl.h"
#include "board.h"

void board_init() {
    gl_init(224, 288, GL_DOUBLEBUFFER);
    //This is the surrounding border
    gl_draw_line(4, 3*8 + 4, 13*8 + 4, 3*8 + 4, GL_BLUE);
    gl_draw_line(14*8 + 4, 3*8 + 4, 27*8 + 4, 3*8 + 4, GL_BLUE);
    gl_draw_line(14*8 + 4, 3*8 + 4, 14*8 + 4, 7*8 + 4, GL_BLUE);
    gl_draw_line(13*8 + 4, 3*8 + 4, 13*8 + 4, 7*8 + 4, GL_BLUE);
    gl_draw_line(13*8 + 4, 7*8 + 4, 14*8 + 4, 7*8 + 4, GL_BLUE);
    gl_draw_line(4, 3*8 + 4, 4, 12*8 + 4, GL_BLUE);
    gl_draw_line(27*8 + 4, 3*8 + 4, 27*8 + 4, 12*8 + 4, GL_BLUE);
    gl_draw_line(4, 12*8 + 4, 5*8 + 4, 12*8 + 4, GL_BLUE);
    gl_draw_line(22*8 + 4, 12*8 + 4, 27*8 + 4, 12*8 + 4, GL_BLUE);
    gl_draw_line(5*8 + 4, 12*8 + 4, 5*8 + 4, 16*8 + 4, GL_BLUE);
    gl_draw_line(22*8 + 4, 12*8 + 4, 22*8 + 4, 16*8 + 4, GL_BLUE);
    gl_draw_line(0, 16*8 + 4, 5*8 + 4, 16*8 + 4, GL_BLUE);
    gl_draw_line(22*8 + 4, 16*8 + 4, 224, 16*8 + 4, GL_BLUE);
    gl_draw_line(0, 18*8 + 4, 5*8 + 4, 18*8 + 4, GL_BLUE);
    gl_draw_line(22*8 + 4, 18*8 + 4, 224, 18*8 + 4, GL_BLUE);
    gl_draw_line(5*8 + 4, 18*8 + 4, 5*8 + 4, 22*8 + 4, GL_BLUE);
    gl_draw_line(22*8 + 4, 18*8 + 4, 22*8 + 4, 22*8 + 4, GL_BLUE);
    gl_draw_line(4, 22*8 + 4, 5*8 + 4, 22*8 + 4, GL_BLUE);
    gl_draw_line(22*8 + 4, 22*8 + 4, 27*8 + 4, 22*8 + 4, GL_BLUE);
    gl_draw_line(4, 22*8 + 4, 4, 27*8 + 4, GL_BLUE);
    gl_draw_line(27*8 + 4, 22*8 + 4, 27*8 + 4, 27*8 + 4, GL_BLUE);
    gl_draw_line(4, 27*8 + 4, 2*8 + 4, 27*8 + 4, GL_BLUE);
    gl_draw_line(25*8 + 4, 27*8 + 4, 27*8 + 4, 27*8 + 4, GL_BLUE);
    gl_draw_line(2*8 + 4, 27*8 + 4, 2*8 + 4, 28*8 + 4, GL_BLUE);
    gl_draw_line(25*8 + 4, 27*8 + 4, 25*8 + 4, 28*8 + 4, GL_BLUE);
    gl_draw_line(4, 28*8 + 4, 2*8 + 4, 28*8 + 4, GL_BLUE);
    gl_draw_line(25*8 + 4, 28*8 + 4, 27*8 + 4, 28*8 + 4, GL_BLUE);
    gl_draw_line(4, 28*8 + 4, 4, 33*8 + 4, GL_BLUE);
    gl_draw_line(27*8 + 4, 28*8 + 4, 27*8 + 4, 33*8 + 4, GL_BLUE);
    gl_draw_line(4, 33*8 + 4, 27*8 + 4, 33*8 + 4, GL_BLUE);

    //These are the top 4 rectangles on the board (left to right)
    gl_draw_line(2*8 + 4, 5*8 + 4, 5*8 + 4, 5*8 + 4, GL_BLUE);
    gl_draw_line(2*8 + 4, 5*8 + 4, 2*8 + 4, 7*8 + 4, GL_BLUE);
    gl_draw_line(2*8 + 4, 7*8 + 4, 5*8 + 4, 7*8 + 4, GL_BLUE);
    gl_draw_line(5*8 + 4, 5*8 + 4, 5*8 + 4, 7*8 + 4, GL_BLUE);

    gl_draw_line(7*8 + 4, 5*8 + 4, 11*8 + 4, 5*8 + 4, GL_BLUE);
    gl_draw_line(7*8 + 4, 5*8 + 4, 7*8 + 4, 7*8 + 4, GL_BLUE);
    gl_draw_line(7*8 + 4, 7*8 + 4, 11*8 + 4, 7*8 + 4, GL_BLUE);
    gl_draw_line(11*8 + 4, 5*8 + 4, 11*8 + 4, 7*8 + 4, GL_BLUE);

    gl_draw_line(16*8 + 4, 5*8 + 4, 20*8 + 4, 5*8 + 4, GL_BLUE);
    gl_draw_line(16*8 + 4, 5*8 + 4, 16*8 + 4, 7*8 + 4, GL_BLUE);
    gl_draw_line(16*8 + 4, 7*8 + 4, 20*8 + 4, 7*8 + 4, GL_BLUE);
    gl_draw_line(20*8 + 4, 5*8 + 4, 20*8 + 4, 7*8 + 4, GL_BLUE);

    gl_draw_line(22*8 + 4, 5*8 + 4, 25*8 + 4, 5*8 + 4, GL_BLUE);
    gl_draw_line(22*8 + 4, 5*8 + 4, 22*8 + 4, 7*8 + 4, GL_BLUE);
    gl_draw_line(22*8 + 4, 7*8 + 4, 25*8 + 4, 7*8 + 4, GL_BLUE);
    gl_draw_line(25*8 + 4, 5*8 + 4, 25*8 + 4, 7*8 + 4, GL_BLUE);

    //These are the next 5 internal shapes (left to right)
    gl_draw_line(2*8 + 4, 9*8 + 4, 5*8 + 4, 9*8 + 4, GL_BLUE);
    gl_draw_line(2*8 + 4, 9*8 + 4, 2*8 + 4, 10*8 + 4, GL_BLUE);
    gl_draw_line(2*8 + 4, 10*8 + 4, 5*8 + 4, 10*8 + 4, GL_BLUE);
    gl_draw_line(5*8 + 4, 9*8 + 4, 5*8 + 4, 10*8 + 4, GL_BLUE);

    gl_draw_line(7*8 + 4, 9*8 + 4, 8*8 + 4, 9*8 + 4, GL_BLUE);
    gl_draw_line(7*8 + 4, 9*8 + 4, 7*8 + 4, 16*8 + 4, GL_BLUE);
    gl_draw_line(7*8 + 4, 16*8 + 4, 8*8 + 4, 16*8 + 4, GL_BLUE);
    gl_draw_line(8*8 + 4, 9*8 + 4, 8*8 + 4, 12*8 + 4, GL_BLUE);
    gl_draw_line(8*8 + 4, 13*8 + 4, 8*8 + 4, 16*8 + 4, GL_BLUE);
    gl_draw_line(8*8 + 4, 12*8 + 4, 11*8 + 4, 12*8 + 4, GL_BLUE);
    gl_draw_line(8*8 + 4, 13*8 + 4, 11*8 + 4, 13*8 + 4, GL_BLUE);
    gl_draw_line(11*8 + 4, 12*8 + 4, 11*8 + 4, 13*8 + 4, GL_BLUE);

    gl_draw_line(10*8 + 4, 9*8 + 4, 17*8 + 4, 9*8 + 4, GL_BLUE);
    gl_draw_line(10*8 + 4, 9*8 + 4, 10*8 + 4, 10*8 + 4, GL_BLUE);
    gl_draw_line(10*8 + 4, 10*8 + 4, 13*8 + 4, 10*8 + 4, GL_BLUE);
    gl_draw_line(14*8 + 4, 10*8 + 4, 17*8 + 4, 10*8 + 4, GL_BLUE);
    gl_draw_line(17*8 + 4, 9*8 + 4, 17*8 + 4, 10*8 + 4, GL_BLUE);
    gl_draw_line(13*8 + 4, 10*8 + 4, 13*8 + 4, 13*8 + 4, GL_BLUE);
    gl_draw_line(14*8 + 4, 10*8 + 4, 14*8 + 4, 13*8 + 4, GL_BLUE);
    gl_draw_line(13*8 + 4, 13*8 + 4, 14*8 + 4, 13*8 + 4, GL_BLUE);

    gl_draw_line(19*8 + 4, 9*8 + 4, 20*8 + 4, 9*8 + 4, GL_BLUE);
    gl_draw_line(20*8 + 4, 9*8 + 4, 20*8 + 4, 16*8 + 4, GL_BLUE);
    gl_draw_line(19*8 + 4, 16*8 + 4, 20*8 + 4, 16*8 + 4, GL_BLUE);
    gl_draw_line(19*8 + 4, 9*8 + 4, 19*8 + 4, 12*8 + 4, GL_BLUE);
    gl_draw_line(19*8 + 4, 13*8 + 4, 19*8 + 4, 16*8 + 4, GL_BLUE);
    gl_draw_line(16*8 + 4, 12*8 + 4, 19*8 + 4, 12*8 + 4, GL_BLUE);
    gl_draw_line(16*8 + 4, 13*8 + 4, 19*8 + 4, 13*8 + 4, GL_BLUE);
    gl_draw_line(16*8 + 4, 12*8 + 4, 16*8 + 4, 13*8 + 4, GL_BLUE);

    gl_draw_line(22*8 + 4, 9*8 + 4, 25*8 + 4, 9*8 + 4, GL_BLUE);
    gl_draw_line(22*8 + 4, 9*8 + 4, 22*8 + 4, 10*8 + 4, GL_BLUE);
    gl_draw_line(22*8 + 4, 10*8 + 4, 25*8 + 4, 10*8 + 4, GL_BLUE);
    gl_draw_line(25*8 + 4, 9*8 + 4, 25*8 + 4, 10*8 + 4, GL_BLUE);

    //This is the ghost prison box
    gl_draw_line(10*8 + 4, 15*8 + 4, 13*8, 15*8 + 4, GL_BLUE);
    gl_draw_line(15*8, 15*8 + 4, 17*8 + 4, 15*8 + 4, GL_BLUE);
    gl_draw_line(13*8, 15*8 + 4, 15*8, 15*8 + 4, GL_CYAN);
    gl_draw_line(10*8 + 4, 15*8 + 4, 10*8 + 4, 19*8 + 4, GL_BLUE);
    gl_draw_line(10*8 + 4, 19*8 + 4, 17*8 + 4, 19*8 + 4, GL_BLUE);
    gl_draw_line(17*8 + 4, 15*8 + 4, 17*8 + 4, 19*8 + 4, GL_BLUE);

    //These are the next three shapes (left to right)
    gl_draw_line(7*8 + 4, 18*8 + 4, 8*8 + 4, 18*8 + 4, GL_BLUE);
    gl_draw_line(7*8 + 4, 18*8 + 4, 7*8 + 4, 22*8 + 4, GL_BLUE);
    gl_draw_line(7*8 + 4, 22*8 + 4, 8*8 + 4, 22*8 + 4, GL_BLUE);
    gl_draw_line(8*8 + 4, 18*8 + 4, 8*8 + 4, 22*8 + 4, GL_BLUE);

    gl_draw_line(10*8 + 4, 21*8 + 4, 17*8 + 4, 21*8 + 4, GL_BLUE);
    gl_draw_line(10*8 + 4, 21*8 + 4, 10*8 + 4, 22*8 + 4, GL_BLUE);
    gl_draw_line(10*8 + 4, 22*8 + 4, 13*8 + 4, 22*8 + 4, GL_BLUE);
    gl_draw_line(14*8 + 4, 22*8 + 4, 17*8 + 4, 22*8 + 4, GL_BLUE);
    gl_draw_line(17*8 + 4, 21*8 + 4, 17*8 + 4, 22*8 + 4, GL_BLUE);
    gl_draw_line(13*8 + 4, 22*8 + 4, 13*8 + 4, 25*8 + 4, GL_BLUE);
    gl_draw_line(14*8 + 4, 22*8 + 4, 14*8 + 4, 25*8 + 4, GL_BLUE);
    gl_draw_line(13*8 + 4, 25*8 + 4, 14*8 + 4, 25*8 + 4, GL_BLUE);

    gl_draw_line(19*8 + 4, 18*8 + 4, 20*8 + 4, 18*8 + 4, GL_BLUE);
    gl_draw_line(19*8 + 4, 18*8 + 4, 19*8 + 4, 22*8 + 4, GL_BLUE);
    gl_draw_line(19*8 + 4, 22*8 + 4, 20*8 + 4, 22*8 + 4, GL_BLUE);
    gl_draw_line(20*8 + 4, 18*8 + 4, 20*8 + 4, 22*8 + 4, GL_BLUE);

    //These are the next 4 shapes
    gl_draw_line(2*8 + 4, 24*8 + 4, 5*8 + 4, 24*8 + 4, GL_BLUE);
    gl_draw_line(2*8 + 4, 24*8 + 4, 2*8 + 4, 25*8 + 4, GL_BLUE);
    gl_draw_line(2*8 + 4, 25*8 + 4, 4*8 + 4, 25*8 + 4, GL_BLUE);
    gl_draw_line(4*8 + 4, 25*8 + 4, 4*8 + 4, 28*8 + 4, GL_BLUE);
    gl_draw_line(4*8 + 4, 28*8 + 4, 5*8 + 4, 28*8 + 4, GL_BLUE);
    gl_draw_line(5*8 + 4, 24*8 + 4, 5*8 + 4, 28*8 + 4, GL_BLUE);

    gl_draw_line(7*8 + 4, 24*8 + 4, 11*8 + 4, 24*8 + 4, GL_BLUE);
    gl_draw_line(7*8 + 4, 24*8 + 4, 7*8 + 4, 25*8 + 4, GL_BLUE);
    gl_draw_line(7*8 + 4, 25*8 + 4, 11*8 + 4, 25*8 + 4, GL_BLUE);
    gl_draw_line(11*8 + 4, 24*8 + 4, 11*8 + 4, 25*8 + 4, GL_BLUE);

    gl_draw_line(16*8 + 4, 24*8 + 4, 20*8 + 4, 24*8 + 4, GL_BLUE);
    gl_draw_line(16*8 + 4, 24*8 + 4, 16*8 + 4, 25*8 + 4, GL_BLUE);
    gl_draw_line(16*8 + 4, 25*8 + 4, 20*8 + 4, 25*8 + 4, GL_BLUE);
    gl_draw_line(20*8 + 4, 24*8 + 4, 20*8 + 4, 25*8 + 4, GL_BLUE);

    gl_draw_line(22*8 + 4, 24*8 + 4, 25*8 + 4, 24*8 + 4, GL_BLUE);
    gl_draw_line(25*8 + 4, 24*8 + 4, 25*8 + 4, 25*8 + 4, GL_BLUE);
    gl_draw_line(22*8 + 4, 24*8 + 4, 22*8 + 4, 28*8 + 4, GL_BLUE);
    gl_draw_line(22*8 + 4, 28*8 + 4, 23*8 + 4, 28*8 + 4, GL_BLUE);
    gl_draw_line(23*8 + 4, 25*8 + 4, 23*8 + 4, 28*8 + 4, GL_BLUE);
    gl_draw_line(23*8 + 4, 25*8 + 4, 25*8 + 4, 25*8 + 4, GL_BLUE);

    //These are the final 3 shapes on the bottom (left to right)
    gl_draw_line(7*8 + 4, 27*8 + 4, 8*8 + 4, 27*8 + 4, GL_BLUE);
    gl_draw_line(7*8 + 4, 27*8 + 4, 7*8 + 4, 30*8 + 4, GL_BLUE);
    gl_draw_line(8*8 + 4, 27*8 + 4, 8*8 + 4, 30*8 + 4, GL_BLUE);
    gl_draw_line(2*8 + 4, 30*8 + 4, 7*8 + 4, 30*8 + 4, GL_BLUE);
    gl_draw_line(8*8 + 4, 30*8 + 4, 11*8 + 4, 30*8 + 4, GL_BLUE);
    gl_draw_line(2*8 + 4, 30*8 + 4, 2*8 + 4, 31*8 + 4, GL_BLUE);
    gl_draw_line(11*8 + 4, 30*8 + 4, 11*8 + 4, 31*8 + 4, GL_BLUE);
    gl_draw_line(2*8 + 4, 31*8 + 4, 11*8 + 4, 31*8 + 4, GL_BLUE);

    gl_draw_line(10*8 + 4, 27*8 + 4, 17*8 + 4, 27*8 + 4, GL_BLUE);
    gl_draw_line(10*8 + 4, 27*8 + 4, 10*8 + 4, 28*8 + 4, GL_BLUE);
    gl_draw_line(10*8 + 4, 28*8 + 4, 13*8 + 4, 28*8 + 4, GL_BLUE);
    gl_draw_line(14*8 + 4, 28*8 + 4, 17*8 + 4, 28*8 + 4, GL_BLUE);
    gl_draw_line(17*8 + 4, 27*8 + 4, 17*8 + 4, 28*8 + 4, GL_BLUE);
    gl_draw_line(13*8 + 4, 28*8 + 4, 13*8 + 4, 31*8 + 4, GL_BLUE);
    gl_draw_line(14*8 + 4, 28*8 + 4, 14*8 + 4, 31*8 + 4, GL_BLUE);
    gl_draw_line(13*8 + 4, 31*8 + 4, 14*8 + 4, 31*8 + 4, GL_BLUE);

    gl_draw_line(19*8 + 4, 27*8 + 4, 20*8 + 4, 27*8 + 4, GL_BLUE);
    gl_draw_line(19*8 + 4, 27*8 + 4, 19*8 + 4, 30*8 + 4, GL_BLUE);
    gl_draw_line(20*8 + 4, 27*8 + 4, 20*8 + 4, 30*8 + 4, GL_BLUE);
    gl_draw_line(16*8 + 4, 30*8 + 4, 19*8 + 4, 30*8 + 4, GL_BLUE);
    gl_draw_line(20*8 + 4, 30*8 + 4, 25*8 + 4, 30*8 + 4, GL_BLUE);
    gl_draw_line(16*8 + 4, 30*8 + 4, 16*8 + 4, 31*8 + 4, GL_BLUE);
    gl_draw_line(25*8 + 4, 30*8 + 4, 25*8 + 4, 31*8 + 4, GL_BLUE);
    gl_draw_line(16*8 + 4, 31*8 + 4, 25*8 + 4, 31*8 + 4, GL_BLUE);
}

void draw_dots() {
    for (int i = 4; i < 224; i = i + 8) {
        for (int j = 4*8 + 4; j < 12*8 + 4; j = j + 8) {
            if (j != 6*8 + 4 && gl_read_pixel(i, j) != GL_BLUE) {
                gl_draw_pixel(i, j, GL_WHITE);
            }
        } 
    }
    for (int j = 4*8 + 4; j < 33*8 + 4; j = j + 8) {
        if (gl_read_pixel(6*8 + 4, j) != GL_BLUE && gl_read_pixel(6*8 + 4, j) != GL_WHITE) {
            gl_draw_pixel(6*8 + 4, j, GL_WHITE);
        }
        if (gl_read_pixel(21*8 + 4, j) != GL_BLUE && gl_read_pixel(21*8 + 4, j) != GL_WHITE) {
            gl_draw_pixel(21*8 + 4, j, GL_WHITE);
        }
    } 
    for (int i = 4; i < 224; i = i + 8) {
        for (int j = 23*8 + 4; j < 33*8 + 4; j = j + 8) {
            if (!(j == 26*8 + 4 && (i == 8 + 4 || i == 26*8 + 4)) && gl_read_pixel(i, j) != GL_BLUE) {
                gl_draw_pixel(i, j, GL_WHITE);
            }
        }
    }
    gl_draw_pixel(12*8 + 4, 6*8 + 4, GL_WHITE);
    gl_draw_pixel(15*8 + 4, 6*8 + 4, GL_WHITE);
    gl_draw_rect(8, 6*8, 8, 8, GL_WHITE);
    gl_draw_rect(26*8, 6*8, 8, 8, GL_WHITE);
    gl_draw_rect(8, 26*8, 8, 8, GL_WHITE);
    gl_draw_rect(26*8, 26*8, 8, 8, GL_WHITE);
}

