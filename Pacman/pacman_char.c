#include "pacman_char.h"
#include "gl.h"
#include "timer.h"
#include "pacman_keyboard.h"

static int xCoord;
static int yCoord;

static void draw_pacman() {
    gl_draw_rect(xCoord, yCoord, 2*8 - 2, 2*8 - 2, GL_YELLOW);
}

static void erase_pacman() {
    gl_draw_rect(xCoord, yCoord, 2*8 - 2, 2*8 - 2, GL_BLACK);
}

void pacman_init() {
    pacman_keyboard_init();
    xCoord = 14*8 - 8 + 1;
    yCoord = 28*8 - 4 - 2*8 + 1;
    draw_pacman(); 
    timer_delay(3);
}

static unsigned char nextMove = 'r';
static unsigned char curMove = 'r';

static int check_sides(unsigned char direction) {
    int side_hit = 0;
    if (direction == 'r' && gl_read_pixel(xCoord + 1 + 2*8 - 2, yCoord) == GL_BLUE) side_hit = 1;
    if (direction == 'r' && gl_read_pixel(xCoord + 1 + 2*8 - 2, yCoord + 2*8 - 2) == GL_BLUE) side_hit = 1;
    if (direction == 'r' && gl_read_pixel(xCoord + 1 + 2*8 - 2, yCoord + 8 - 1) == GL_BLUE) side_hit = 1;
    if (direction == 'l' && gl_read_pixel(xCoord - 1, yCoord) == GL_BLUE) side_hit = 1;
    if (direction == 'l' && gl_read_pixel(xCoord - 1, yCoord + 2*8 - 2) == GL_BLUE) side_hit = 1;
    if (direction == 'l' && gl_read_pixel(xCoord - 1, yCoord + 8 - 1) == GL_BLUE) side_hit = 1;
    if (direction == 'u' && gl_read_pixel(xCoord, yCoord - 1) == GL_BLUE) side_hit = 1;
    if (direction == 'u' && gl_read_pixel(xCoord + 2*8 - 2, yCoord - 1) == GL_BLUE) side_hit = 1;
    if (direction == 'u' && gl_read_pixel(xCoord + 8 - 1, yCoord - 1) == GL_BLUE) side_hit = 1;
    if (direction == 'd' && gl_read_pixel(xCoord, yCoord + 1 + 2*8 - 2) == GL_BLUE) side_hit = 1;
    if (direction == 'd' && gl_read_pixel(xCoord + 2*8 - 2, yCoord + 1 + 2*8 - 2) == GL_BLUE) side_hit = 1;
    if (direction == 'd' && gl_read_pixel(xCoord + 8 - 1, yCoord + 1 + 2*8 - 2) == GL_BLUE) side_hit = 1;
    if (direction == 'd' && gl_read_pixel(xCoord, yCoord + 1 + 2*8 - 2) == GL_CYAN) side_hit = 1;
    if (direction == 'd' && gl_read_pixel(xCoord + 2*8 - 2, yCoord + 1 + 2*8 - 2) == GL_CYAN) side_hit = 1;
    if (direction == 'd' && gl_read_pixel(xCoord + 8 - 1, yCoord + 1 + 2*8 - 2) == GL_CYAN) side_hit = 1;
    return side_hit;
}

void pacman_move() {
    erase_pacman();
    unsigned char userTyped = pacman_keyboard_read_next();
    if (userTyped == PS2_KEY_ARROW_LEFT) {
        nextMove = 'l';
    } else if (userTyped == PS2_KEY_ARROW_RIGHT) {
        nextMove = 'r';
    } else if (userTyped == PS2_KEY_ARROW_UP) {
        nextMove = 'u';
    } else if (userTyped == PS2_KEY_ARROW_DOWN) {
        nextMove = 'd';
    }
    for (int i = 0; i < 3; i++) {
        if (nextMove == 'r' && !check_sides('r')) curMove = 'r';
        if (nextMove == 'l' && !check_sides('l')) curMove = 'l';
        if (nextMove == 'u' && !check_sides('u')) curMove = 'u';
        if (nextMove == 'd' && !check_sides('d')) curMove = 'd';
        if (curMove == 'r' && !check_sides('r')) xCoord = xCoord + 1;
        if (curMove == 'l' && !check_sides('l')) xCoord = xCoord - 1;
        if (curMove == 'u' && !check_sides('u')) yCoord = yCoord - 1;
        if (curMove == 'd' && !check_sides('d')) yCoord = yCoord + 1;
    }
    draw_pacman();
    gl_swap_buffer();
}
