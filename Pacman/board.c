#include "gl.c"
#include "board.h"

void board_init() {
    gl_init(224, 288, GL_DOUBLEBUFFER);
    gl_draw_line(4, 3*8 + 4, 13*8 + 4, 3*8 + 4, GL_BLUE);
    gl_draw_line(14*8 + 4, 3*8 + 4, 27*8 + 4, 3*8 + 4, GL_BLUE);
    gl_draw_line(14*8 + 4, 3*8 + 4, 14*8 + 4, 7*8 + 4, GL_BLUE);
    gl_draw_line(13*8 + 4, 3*8 + 4, 13*8 + 4, 7*8 + 4, GL_BLUE);
    gl_draw_line(13*8 + 4, 7*8 + 4, 14*8 + 4, 7*8 + 4, GL_BLUE);
}

