#include "ghosts.h"
#include "gl.h"
#include "pacman_char.h"

static int blinky_x;
static int blinky_y;

static color_t saveRect[2*8 - 2][2*8 - 2];

static void draw_blinky() {
    for (int i = 0; i < 2*8 - 2; i++) {
        for (int j = 0; j < 2*8 - 2; j++) {
            saveRect[i][j] = gl_read_pixel(blinky_x + i, blinky_y + j);
        }  
    } 
    gl_draw_rect(blinky_x, blinky_y, 2*8 - 2, 2*8 - 2, GL_RED);
}

static void erase_blinky() {
    for (int i = 0; i < 2*8 - 2; i++) {
        for (int j = 0; j < 2*8 - 2; j++) {
            gl_draw_pixel(blinky_x + i, blinky_y + j, saveRect[i][j]);
        }  
    }
}

void ghosts_init() {
    blinky_x = 14*8 - 8 + 1;
    blinky_y = 14*8 - 4 + 1;
    draw_blinky();
}

static unsigned char blinkyNextMove = 'l';
static unsigned char blinkyCurMove = 'l';

static void ghost_decision(int x, int y) {
    int checkRight = 0;
    int checkLeft = 0;
    int checkUp = 0;
    int checkDown = 0;
    int count = 0;
    unsigned char dontGo;
    if (blinkyCurMove == 'r') dontGo = 'l';
    if (blinkyCurMove == 'l') dontGo = 'r';
    if (blinkyCurMove == 'd') dontGo = 'u';
    if (blinkyCurMove == 'u') dontGo = 'd';
    if (dontGo != 'r' && !check_sides(blinky_x, blinky_y, 'r', GL_BLUE)) {
        checkRight = 1;
        count++;
    }
    if (dontGo != 'l' && !check_sides(blinky_x, blinky_y, 'l', GL_BLUE)) {
        checkLeft = 1;
        count++;
    }
    if (dontGo != 'u' && !check_sides(blinky_x, blinky_y, 'u',GL_BLUE)) {
        checkUp = 1;
        count++;
    }
    if (dontGo != 'd' && !check_sides(blinky_x, blinky_y, 'd', GL_BLUE)) {
        checkDown = 1;
        count++;
    }
    int leastDistance = 0;
    if (checkRight == 1) {
        double distance = (blinky_x + 2*8 + 4 - pacman_get_x()) * (blinky_x + 2*8 + 4 - pacman_get_x()) + (blinky_y + 4 - pacman_get_y()) * (blinky_y + 4 - pacman_get_y());
        if (leastDistance == 0 || distance < leastDistance) {
            blinkyCurMove = 'r';
            leastDistance = distance;
        }
    }
    if (checkLeft == 1) {
        int distance = (blinky_x - 4 - pacman_get_x()) * (blinky_x - 4 - pacman_get_x()) + (blinky_y + 4 - pacman_get_y()) * (blinky_y + 4 - pacman_get_y());
        if (leastDistance == 0 || distance < leastDistance) {
            blinkyCurMove = 'l';
            leastDistance = distance;
        }
    }
    if (checkUp == 1) {
        int distance = (blinky_x + 4 - pacman_get_x()) * (blinky_x + 4 - pacman_get_x()) + (blinky_y - 4 - pacman_get_y()) * (blinky_y - 4 - pacman_get_y());
        if (leastDistance == 0 || distance < leastDistance) {
            blinkyCurMove = 'u';
            leastDistance = distance;
        }
    }
    if (checkDown == 1) {
        int distance = (blinky_x + 4 - pacman_get_x()) * (blinky_x + 4 - pacman_get_x()) + (blinky_y + 2*8 + 4 - pacman_get_y()) * (blinky_y + 2*8 + 4 - pacman_get_y());
        if (leastDistance == 0 || distance < leastDistance) {
            blinkyCurMove = 'd';
            leastDistance = distance;
        }
    }
}

void blinky_move() {
    erase_blinky();
    for (int i = 0; i < 3; i++) {
        ghost_decision(pacman_get_x(), pacman_get_y());
        if (blinkyCurMove == 'r' && !check_sides(blinky_x, blinky_y, 'r', GL_BLUE)) blinky_x = blinky_x + 1;
        if (blinkyCurMove == 'l' && !check_sides(blinky_x, blinky_y, 'l', GL_BLUE)) blinky_x = blinky_x - 1;
        if (blinkyCurMove == 'u' && !check_sides(blinky_x, blinky_y, 'u',GL_BLUE)) blinky_y = blinky_y - 1;
        if (blinkyCurMove == 'd' && !check_sides(blinky_x, blinky_y, 'd', GL_BLUE)) blinky_y = blinky_y + 1;
        if (blinky_x <= 0) blinky_x = gl_get_width() - 1;
        if (blinky_x >= gl_get_width()) blinky_x = 1;
    }
    draw_blinky();
}
