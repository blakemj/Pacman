#include "pacman_char.h"
#include "gl.h"
#include "timer.h"
#include "pacman_keyboard.h"
#include "gl_more.h"
#include "printf.h"

#define pacman_width 2*8 - 2

static int xCoord;
static int yCoord;

int pacman_get_x() {
    return xCoord;
}

int pacman_get_y() {
    return yCoord;
}

int numDots;

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

int check_sides(int x, int y, unsigned char direction, color_t color) {
    int side_hit = 0;
    if (direction == 'r' && gl_read_pixel(x + 1 + 2*8 - 2, y) == color) side_hit = 1;
    if (direction == 'r' && gl_read_pixel(x + 1 + 2*8 - 2, y + 2*8 - 2) == color) side_hit = 1;
    if (direction == 'r' && gl_read_pixel(x + 1 + 2*8 - 2, y + 8 - 1) == color) side_hit = 1;
    if (direction == 'l' && gl_read_pixel(x - 1, y) == color) side_hit = 1;
    if (direction == 'l' && gl_read_pixel(x - 1, y + 2*8 - 2) == color) side_hit = 1;
    if (direction == 'l' && gl_read_pixel(x - 1, y + 8 - 1) == color) side_hit = 1;
    if (direction == 'u' && gl_read_pixel(x, y - 1) == color) side_hit = 1;
    if (direction == 'u' && gl_read_pixel(x + 2*8 - 2, y - 1) == color) side_hit = 1;
    if (direction == 'u' && gl_read_pixel(x + 8 - 1, y - 1) == color) side_hit = 1;
    if (direction == 'd' && gl_read_pixel(x, y + 1 + 2*8 - 2) == color) side_hit = 1;
    if (direction == 'd' && gl_read_pixel(x + 2*8 - 2, y + 1 + 2*8 - 2) == color) side_hit = 1;
    if (direction == 'd' && gl_read_pixel(x + 8 - 1, y + 1 + 2*8 - 2) == color) side_hit = 1;
    if (color == GL_BLUE && direction == 'd' && gl_read_pixel(x, y + 1 + 2*8 - 2) == GL_CYAN) side_hit = 1;
    if (color == GL_BLUE && direction == 'd' && gl_read_pixel(x + 2*8 - 2, y + 1 + 2*8 - 2) == GL_CYAN) side_hit = 1;
    if (color == GL_BLUE && direction == 'd' && gl_read_pixel(x + 8 - 1, y + 1 + 2*8 - 2) == GL_CYAN) side_hit = 1;
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
        if (nextMove == 'r' && !check_sides(xCoord, yCoord, 'r', GL_BLUE)) curMove = 'r';
        if (nextMove == 'l' && !check_sides(xCoord, yCoord, 'l', GL_BLUE)) curMove = 'l';
        if (nextMove == 'u' && !check_sides(xCoord, yCoord, 'u', GL_BLUE)) curMove = 'u';
        if (nextMove == 'd' && !check_sides(xCoord, yCoord, 'd', GL_BLUE)) curMove = 'd';
        if (check_sides(xCoord, yCoord, curMove, GL_WHITE)) numDots--;
        if (curMove == 'r' && !check_sides(xCoord, yCoord, 'r', GL_BLUE)) xCoord = xCoord + 1;
        if (curMove == 'l' && !check_sides(xCoord, yCoord, 'l', GL_BLUE)) xCoord = xCoord - 1;
        if (curMove == 'u' && !check_sides(xCoord, yCoord, 'u', GL_BLUE)) yCoord = yCoord - 1;
        if (curMove == 'd' && !check_sides(xCoord, yCoord, 'd', GL_BLUE)) yCoord = yCoord + 1;
        if (xCoord <= 0) xCoord = gl_get_width() - 1;
        if (xCoord >= gl_get_width()) xCoord = 1;
    }
    draw_pacman();
}
