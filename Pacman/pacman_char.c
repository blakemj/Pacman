#include "pacman_char.h"
#include "gl.h"
#include "timer.h"

static int xCoord;
static int yCoord;

static void draw_pacman() {
    gl_draw_rect(xCoord, yCoord, 2*8 - 4, 2*8 - 4, GL_YELLOW);
}

static void erase_pacman() {
    gl_draw_rect(xCoord, yCoord, 2*8 - 4, 2*8 - 4, GL_BLACK);
}

void pacman_init() {
    xCoord = 14*8 - 8 + 2;
    yCoord = 28*8 - 4 - 2*8 + 2;
    draw_pacman(); 
}

void pacman_move() {
    erase_pacman();
    for (int i = 0; i < 3; i++) {
        if (gl_read_pixel(xCoord + 1 + 2*8 - 4, yCoord) != GL_BLUE) xCoord = xCoord + 1;
    }
    draw_pacman();
    gl_swap_buffer();
}
