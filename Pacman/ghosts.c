#include "ghosts.h"
#include "gl.h"
#include "pacman_char.h"
#include "timer.h"
#include "board.h"

static int blinky_x;
static int blinky_y;
static int pinky_x;
static int pinky_y;
static int inky_x;
static int inky_y;
static int clyde_x;
static int clyde_y;

static unsigned char blinkyCurMove;
static unsigned char pinkyCurMove;
static unsigned char inkyCurMove;
static unsigned char clydeCurMove;

static int blinkyStuckInBox;
static int pinkyStuckInBox;
static int inkyStuckInBox;
static int clydeStuckInBox;

static color_t saveRectBlinky[2*8 - 2][2*8 - 2];
static color_t saveRectPinky[2*8 - 2][2*8 - 2];
static color_t saveRectInky[2*8 - 2][2*8 - 2];
static color_t saveRectClyde[2*8 - 2][2*8 - 2];

static void draw_blinky_rect() {
        gl_draw_rect(blinky_x, blinky_y, 2*8 - 2, 2*8 - 2, GL_RED);
}

static void draw_pinky_rect() {
        gl_draw_rect(pinky_x, pinky_y, 2*8 - 2, 2*8 - 2, GL_MAGENTA);
}

static void draw_inky_rect() {
        gl_draw_rect(inky_x, inky_y, 2*8 - 2, 2*8 - 2, GL_CYAN);
}

static void draw_clyde_rect() {
        gl_draw_rect(clyde_x, clyde_y, 2*8 - 2, 2*8 - 2, GL_AMBER);
}

static void save_under_blinky() {
    for (int i = 0; i < 2*8 - 2; i++) {
        for (int j = 0; j < 2*8 - 2; j++) {
             saveRectBlinky[i][j] = gl_read_pixel(blinky_x + i, blinky_y + j);
        }
    }
}

static void save_under_pinky() {
    for (int i = 0; i < 2*8 - 2; i++) {
        for (int j = 0; j < 2*8 - 2; j++) {
             saveRectPinky[i][j] = gl_read_pixel(pinky_x + i, pinky_y + j);
        }
    }
}

static void save_under_inky() {
    for (int i = 0; i < 2*8 - 2; i++) {
        for (int j = 0; j < 2*8 - 2; j++) {
             saveRectInky[i][j] = gl_read_pixel(inky_x + i, inky_y + j);
        }
    }
}

static void save_under_clyde() {
    for (int i = 0; i < 2*8 - 2; i++) {
        for (int j = 0; j < 2*8 - 2; j++) {
             saveRectClyde[i][j] = gl_read_pixel(clyde_x + i, clyde_y + j);
        }
    }
}

void erase_blinky() {
    for (int i = 0; i < 2*8 - 2; i++) {
        for (int j = 0; j < 2*8 - 2; j++) {
            gl_draw_pixel(blinky_x + i, blinky_y + j, saveRectBlinky[i][j]);
        }  
    }
}

void erase_pinky() {
    for (int i = 0; i < 2*8 - 2; i++) {
        for (int j = 0; j < 2*8 - 2; j++) {
            gl_draw_pixel(pinky_x + i, pinky_y + j, saveRectPinky[i][j]);
        }
    }
}

void erase_inky() {
    for (int i = 0; i < 2*8 - 2; i++) {
        for (int j = 0; j < 2*8 - 2; j++) {
            gl_draw_pixel(inky_x + i, inky_y + j, saveRectInky[i][j]);
        }
    }
}

void erase_clyde() {
    for (int i = 0; i < 2*8 - 2; i++) {
        for (int j = 0; j < 2*8 - 2; j++) {
            gl_draw_pixel(clyde_x + i, clyde_y + j, saveRectClyde[i][j]);
        }
    }
}

static void draw_clyde() {
    save_under_clyde();
    save_under_inky();
    save_under_blinky();
    save_under_pinky();
    draw_blinky_rect();
    draw_pinky_rect();
    draw_inky_rect();
    draw_clyde_rect();
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
        if (inkyCurMove == 'r') inkyCurMove = 'l';
        if (inkyCurMove == 'l') inkyCurMove = 'r';
        if (inkyCurMove == 'd') inkyCurMove = 'u';
        if (inkyCurMove == 'u') inkyCurMove = 'd';
        if (clydeCurMove == 'r') clydeCurMove = 'l';
        if (clydeCurMove == 'l') clydeCurMove = 'r';
        if (clydeCurMove == 'd') clydeCurMove = 'u';
        if (clydeCurMove == 'u') clydeCurMove = 'd';
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
    pinky_x = 14*8 - 8 + 1;
    pinky_y = 17*8 - 4 + 1;
    pinkyCurMove = 'u';
    pinkyStuckInBox = timer_get_ticks();
    inky_x = 12*8 - 8 + 1;
    inky_y = 17*8 - 4 + 1;
    inkyCurMove = 'u';
    inkyStuckInBox = 0;
    clyde_x = 16*8 - 8 + 1;
    clyde_y = 17*8 - 4 + 1;
    clydeCurMove = 'u';
    clydeStuckInBox = 0;
    draw_clyde();
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
}

void pinky_move() {
    erase_pinky();
    if (timer_get_ticks() - pinkyStuckInBox < 4 * 1000000) return;
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
}

void inky_move() {
    erase_inky();
    if (244 - numDots < 20) return;
    for (int i = 0; i < 2; i++) {
        if (inkyStuckInBox < 16) {
            inkyCurMove = 'r';
            inkyStuckInBox++;
        } else if (inkyStuckInBox < 40) {
            inkyCurMove = 'u';
            inkyStuckInBox++;
        } else if (scatter) {
            inkyCurMove = ghost_decision(inky_x, inky_y, inkyCurMove, 27*8, 35*8);
        } else {
            int targetX = pacman_get_x();
            int targetY = pacman_get_y();
            if (pacman_get_curMove() == 'r') targetX = targetX + 2*8;
            if (pacman_get_curMove() == 'l') targetX = targetX - 2*8;
            if (pacman_get_curMove() == 'd') targetY = targetY - 2*8;
            if (pacman_get_curMove() == 'u') targetY = targetY - 2*8;
            targetX = blinky_x + (targetX - blinky_x) * 2;
            targetY = blinky_y + (targetY - blinky_y) * 2;
            inkyCurMove = ghost_decision(inky_x, inky_y, inkyCurMove, targetX, targetY);
        }
        if (inkyCurMove == 'r' && !check_sides(inky_x, inky_y, 'r', GL_BLUE)) inky_x = inky_x + 1;
        if (inkyCurMove == 'l' && !check_sides(inky_x, inky_y, 'l', GL_BLUE)) inky_x = inky_x - 1;
        if (inkyCurMove == 'u' && !check_sides(inky_x, inky_y, 'u',GL_BLUE)) inky_y = inky_y - 1;
        if (inkyCurMove == 'd' && !check_sides(inky_x, inky_y, 'd', GL_BLUE)) inky_y = inky_y + 1;
        if (inky_x <= 0) inky_x = gl_get_width() - 1;
        if (inky_x >= gl_get_width()) inky_x = 1;
    }
}

void clyde_move() {
    erase_clyde();
    if (!(244 - numDots < 244 / 3 - 15)) {
    for (int i = 0; i < 2; i++) {
        if (clydeStuckInBox < 16) {
            clydeCurMove = 'l';
            clydeStuckInBox++;
        } else if (clydeStuckInBox < 40) {
            clydeCurMove = 'u';
            clydeStuckInBox++;
        } else if (scatter) {
            clydeCurMove = ghost_decision(clyde_x, clyde_y, clydeCurMove, 0, 35*8);
        } else {
            int targetX = pacman_get_x();
            int targetY = pacman_get_y();
            int distance = (targetX - clyde_x) * (targetX - clyde_x) + (targetY - clyde_y) * (targetY - clyde_y);
            if (distance > 8 * 8) {
                clydeCurMove = ghost_decision(clyde_x, clyde_y, clydeCurMove, targetX, targetY);
            } else {
                clydeCurMove = ghost_decision(clyde_x, clyde_y, clydeCurMove, 0, 35*8);
            }
        }
        if (clydeCurMove == 'r' && !check_sides(clyde_x, clyde_y, 'r', GL_BLUE)) clyde_x = clyde_x + 1;
        if (clydeCurMove == 'l' && !check_sides(clyde_x, clyde_y, 'l', GL_BLUE)) clyde_x = clyde_x - 1;
        if (clydeCurMove == 'u' && !check_sides(clyde_x, clyde_y, 'u',GL_BLUE)) clyde_y = clyde_y - 1;
        if (clydeCurMove == 'd' && !check_sides(clyde_x, clyde_y, 'd', GL_BLUE)) clyde_y = clyde_y + 1;
        if (clyde_x <= 0) clyde_x = gl_get_width() - 1;
        if (clyde_x >= gl_get_width()) clyde_x = 1;
    }
    }
    draw_clyde();
}
