#include "ghosts.h"
#include "gl.h"
#include "pacman_char.h"
#include "timer.h"

static int blinky_x;
static int blinky_y;

static unsigned char blinkyCurMove;

static color_t saveRectBlinky[2*8 - 2][2*8 - 2];

static void draw_blinky() {
    for (int i = 0; i < 2*8 - 2; i++) {
        for (int j = 0; j < 2*8 - 2; j++) {
            if (gl_read_pixel(blinky_x + i, blinky_y + j) != GL_MAGENTA) {
                saveRectBlinky[i][j] = gl_read_pixel(blinky_x + i, blinky_y + j);
            } else {
                saveRectBlinky[i][j] = GL_BLACK;
            }
        }  
    } 
    gl_draw_rect(blinky_x, blinky_y, 2*8 - 2, 2*8 - 2, GL_RED);
}

void erase_blinky() {
    for (int i = 0; i < 2*8 - 2; i++) {
        for (int j = 0; j < 2*8 - 2; j++) {
            gl_draw_pixel(blinky_x + i, blinky_y + j, saveRectBlinky[i][j]);
        }  
    }
}

static int pinky_x;
static int pinky_y;

static unsigned char pinkyCurMove;
static int pinkyStuckInBox;

static color_t saveRectPinky[2*8 - 2][2*8 - 2];

static void draw_pinky() {
    erase_blinky();
    for (int i = 0; i < 2*8 - 2; i++) {
        for (int j = 0; j < 2*8 - 2; j++) {
            if (gl_read_pixel(pinky_x + i, pinky_y + j) != GL_RED) {
                saveRectPinky[i][j] = gl_read_pixel(pinky_x + i, pinky_y + j);
            } else {
                saveRectPinky[i][j] = GL_BLACK;
            }
        }
    }
    draw_blinky();
    gl_draw_rect(pinky_x, pinky_y, 2*8 - 2, 2*8 - 2, GL_MAGENTA);
}

void erase_pinky() {
    for (int i = 0; i < 2*8 - 2; i++) {
        for (int j = 0; j < 2*8 - 2; j++) {
            gl_draw_pixel(pinky_x + i, pinky_y + j, saveRectPinky[i][j]);
        }
    }
}

static int mode_start;
static int scatter;
static int modeTime;
static int modeRound;

static void mode_change() {
    if (modeRound == 4) {
        scatter = 0;
        return;
    }
    if (timer_get_ticks() - mode_start > modeTime * 1000000) {
        scatter = !scatter;
        if (blinkyCurMove == 'r') blinkyCurMove = 'l';
        if (blinkyCurMove == 'l') blinkyCurMove = 'r';
        if (blinkyCurMove == 'd') blinkyCurMove = 'u';
        if (blinkyCurMove == 'u') blinkyCurMove = 'd';
        if (pinkyCurMove == 'r') pinkyCurMove = 'l';
        if (pinkyCurMove == 'l') pinkyCurMove = 'r';
        if (pinkyCurMove == 'd') pinkyCurMove = 'u';
        if (pinkyCurMove == 'u') pinkyCurMove = 'd';
        if (modeTime < 20) {
            modeTime = 20;
            modeRound++;
        } else {
            modeTime = 12 - 3 * modeRound;
        }
        mode_start = timer_get_ticks();
    }
}

void ghosts_init() {
    blinky_x = 14*8 - 8 + 1;
    blinky_y = 14*8 - 4 + 1;
    blinkyCurMove = 'l';
    draw_blinky();
    pinky_x = 14*8 - 8 + 1;
    pinky_y = 17*8 - 4 + 1;
    pinkyCurMove = 'u';
    pinkyStuckInBox = timer_get_ticks();
    draw_pinky();
    scatter = 1;
    modeTime = 12;
    modeRound = 0;
    mode_start = timer_get_ticks();
}

static unsigned char ghost_decision(int ghostx, int ghosty, unsigned char curMove, int targetx, int targety) {
    int checkRight = 0;
    int checkLeft = 0;
    int checkUp = 0;
    int checkDown = 0;
    unsigned char dontGo;
    if (curMove == 'r') dontGo = 'l';
    if (curMove == 'l') dontGo = 'r';
    if (curMove == 'd') dontGo = 'u';
    if (curMove == 'u') dontGo = 'd';
    if (dontGo != 'r' && !check_sides(ghostx, ghosty, 'r', GL_BLUE)) checkRight = 1;
    if (dontGo != 'l' && !check_sides(ghostx, ghosty, 'l', GL_BLUE)) checkLeft = 1;
    if (dontGo != 'u' && !check_sides(ghostx, ghosty, 'u',GL_BLUE)) checkUp = 1;
    if (dontGo != 'd' && !check_sides(ghostx, ghosty, 'd', GL_BLUE)) checkDown = 1;
    int leastDistance = 0;
    if (checkRight == 1) {
        int distance = (ghostx + 2*8 + 4 - targetx) * (ghostx + 2*8 + 4 - targetx) + (ghosty + 4 - targety) * (ghosty + 4 - targety);
        if (leastDistance == 0 || distance < leastDistance) {
            curMove = 'r';
            leastDistance = distance;
        }
    }
    if (checkLeft == 1) {
        int distance = (ghostx - 4 - targetx) * (ghostx - 4 - targetx) + (ghosty + 4 - targety) * (ghosty + 4 - targety);
        if (leastDistance == 0 || distance < leastDistance) {
            curMove = 'l';
            leastDistance = distance;
        }
    }
    if (checkUp == 1) {
        int distance = (ghostx + 4 - targetx) * (ghostx + 4 - targetx) + (ghosty - 4 - targety) * (ghosty - 4 - targety);
        if (leastDistance == 0 || distance < leastDistance) {
            curMove = 'u';
            leastDistance = distance;
        }
    }
    if (checkDown == 1) {
        int distance = (ghostx + 4 - targetx) * (ghostx + 4 - targetx) + (ghosty + 2*8 + 4 - targety) * (ghosty + 2*8 + 4 - targety);
        if (leastDistance == 0 || distance < leastDistance) {
            curMove = 'd';
            leastDistance = distance;
        }
    }
    return curMove;
}

void blinky_move() {
    erase_blinky();
    for (int i = 0; i < 2; i++) {
        mode_change();
        if (scatter) {
            blinkyCurMove = ghost_decision(blinky_x, blinky_y, blinkyCurMove, 25*8, 0);
        } else {
            blinkyCurMove = ghost_decision(blinky_x, blinky_y, blinkyCurMove, pacman_get_x(), pacman_get_y());
        }
        if (blinkyCurMove == 'r' && !check_sides(blinky_x, blinky_y, 'r', GL_BLUE)) blinky_x = blinky_x + 1;
        if (blinkyCurMove == 'l' && !check_sides(blinky_x, blinky_y, 'l', GL_BLUE)) blinky_x = blinky_x - 1;
        if (blinkyCurMove == 'u' && !check_sides(blinky_x, blinky_y, 'u',GL_BLUE)) blinky_y = blinky_y - 1;
        if (blinkyCurMove == 'd' && !check_sides(blinky_x, blinky_y, 'd', GL_BLUE)) blinky_y = blinky_y + 1;
        if (blinky_x <= 0) blinky_x = gl_get_width() - 1;
        if (blinky_x >= gl_get_width()) blinky_x = 1;
    }
    draw_blinky();
}

void pinky_move() {
    if (timer_get_ticks() - pinkyStuckInBox < 4 * 1000000) return;
    erase_pinky();
    for (int i = 0; i < 2; i++) {
        if (scatter) {
            pinkyCurMove = ghost_decision(pinky_x, pinky_y, pinkyCurMove, 2*8, 0);
        } else {
            int targetX = pacman_get_x();
            int targetY = pacman_get_y();
            if (pacman_get_curMove() == 'r') targetX = targetX + 4*8;
            if (pacman_get_curMove() == 'l') targetX = targetX - 4*8;
            if (pacman_get_curMove() == 'd') targetY = targetY - 4*8;
            if (pacman_get_curMove() == 'u') targetY = targetY - 4*8;
            pinkyCurMove = ghost_decision(pinky_x, pinky_y, pinkyCurMove, targetX, targetY);
        }
        if (pinkyCurMove == 'r' && !check_sides(pinky_x, pinky_y, 'r', GL_BLUE)) pinky_x = pinky_x + 1;
        if (pinkyCurMove == 'l' && !check_sides(pinky_x, pinky_y, 'l', GL_BLUE)) pinky_x = pinky_x - 1;
        if (pinkyCurMove == 'u' && !check_sides(pinky_x, pinky_y, 'u',GL_BLUE)) pinky_y = pinky_y - 1;
        if (pinkyCurMove == 'd' && !check_sides(pinky_x, pinky_y, 'd', GL_BLUE)) pinky_y = pinky_y + 1;
        if (pinky_x <= 0) pinky_x = gl_get_width() - 1;
        if (pinky_x >= gl_get_width()) pinky_x = 1;
    }
    draw_pinky();
}
