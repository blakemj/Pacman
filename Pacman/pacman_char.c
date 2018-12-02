#include "pacman_char.h"
#include "gl.h"
#include "timer.h"
#include "pacman_keyboard.h"
#include "gl_more.h"
#include "printf.h"

#define pacman_width 2*8 - 2

static int xCoord;
static int yCoord;

static int numDots;

static void draw_pacman() {
    gl_draw_circle(xCoord, yCoord, pacman_width, pacman_width, GL_YELLOW);
}

static void erase_pacman() {
    gl_draw_rect(xCoord, yCoord, 2*8 - 2, 2*8 - 2, GL_BLACK);
}

void pacman_init() {
    pacman_keyboard_init();
    xCoord = 14*8 - 8 + 1;
    yCoord = 28*8 - 4 - 2*8 + 1;
    numDots = 244;
    draw_pacman(); 
}

static unsigned char nextMove = 'r';
static unsigned char curMove = 'r';

static int check_sides(unsigned char direction, color_t color) {
    int side_hit = 0;
    if (direction == 'r' && gl_read_pixel(xCoord + 1 + 2*8 - 2, yCoord) == color) side_hit = 1;
    if (direction == 'r' && gl_read_pixel(xCoord + 1 + 2*8 - 2, yCoord + 2*8 - 2) == color) side_hit = 1;
    if (direction == 'r' && gl_read_pixel(xCoord + 1 + 2*8 - 2, yCoord + 8 - 1) == color) side_hit = 1;
    if (direction == 'l' && gl_read_pixel(xCoord - 1, yCoord) == color) side_hit = 1;
    if (direction == 'l' && gl_read_pixel(xCoord - 1, yCoord + 2*8 - 2) == color) side_hit = 1;
    if (direction == 'l' && gl_read_pixel(xCoord - 1, yCoord + 8 - 1) == color) side_hit = 1;
    if (direction == 'u' && gl_read_pixel(xCoord, yCoord - 1) == color) side_hit = 1;
    if (direction == 'u' && gl_read_pixel(xCoord + 2*8 - 2, yCoord - 1) == color) side_hit = 1;
    if (direction == 'u' && gl_read_pixel(xCoord + 8 - 1, yCoord - 1) == color) side_hit = 1;
    if (direction == 'd' && gl_read_pixel(xCoord, yCoord + 1 + 2*8 - 2) == color) side_hit = 1;
    if (direction == 'd' && gl_read_pixel(xCoord + 2*8 - 2, yCoord + 1 + 2*8 - 2) == color) side_hit = 1;
    if (direction == 'd' && gl_read_pixel(xCoord + 8 - 1, yCoord + 1 + 2*8 - 2) == color) side_hit = 1;
    if (color == GL_BLUE && direction == 'd' && gl_read_pixel(xCoord, yCoord + 1 + 2*8 - 2) == GL_CYAN) side_hit = 1;
    if (color == GL_BLUE && direction == 'd' && gl_read_pixel(xCoord + 2*8 - 2, yCoord + 1 + 2*8 - 2) == GL_CYAN) side_hit = 1;
    if (color == GL_BLUE && direction == 'd' && gl_read_pixel(xCoord + 8 - 1, yCoord + 1 + 2*8 - 2) == GL_CYAN) side_hit = 1;
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
    for (int i = 0; i < 5; i++) {
        if (nextMove == 'r' && !check_sides('r', GL_BLUE)) curMove = 'r';
        if (nextMove == 'l' && !check_sides('l', GL_BLUE)) curMove = 'l';
        if (nextMove == 'u' && !check_sides('u', GL_BLUE)) curMove = 'u';
        if (nextMove == 'd' && !check_sides('d', GL_BLUE)) curMove = 'd';
        if (check_sides(curMove, GL_WHITE)) numDots--;
        if (curMove == 'r' && !check_sides('r', GL_BLUE)) xCoord = xCoord + 1;
        if (curMove == 'l' && !check_sides('l', GL_BLUE)) xCoord = xCoord - 1;
        if (curMove == 'u' && !check_sides('u', GL_BLUE)) yCoord = yCoord - 1;
        if (curMove == 'd' && !check_sides('d', GL_BLUE)) yCoord = yCoord + 1;
        if (xCoord <= 0) xCoord = gl_get_width() - 1;
        if (xCoord >= gl_get_width()) xCoord = 1;
    }
    draw_pacman();
    gl_swap_buffer();
}
