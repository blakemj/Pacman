#include "pacman_char.h"
#include "gl.h"
#include "timer.h"
#include "pacman_keyboard.h"
#include "gl_more.h"
#include "printf.h"
#include "board.h"
#include "ghosts.h"
#include "nes.h"

#define pacman_width (2*8 - 2)

static int xCoord;
static int yCoord;
static int msxCoord;
static int msyCoord;

static int ghostHit;
static int msghostHit;

int frightened_points;
int superDotBonus;
int frightenedPointsNotAdded;

static unsigned char nextMove;
static unsigned char curMove;
static unsigned char msnextMove;
static unsigned char mscurMove;

int ms_pacman_hit_ghost() {
    return msghostHit;
}

int ms_pacman_get_x() {
    return msxCoord;
}

int ms_pacman_get_y() {
    return msyCoord;
}

int pacman_hit_ghost() {
    return ghostHit;
}

int pacman_get_x() {
    return xCoord;
}

int pacman_get_y() {
    return yCoord;
}

void draw_ms_pacman(int x, int y) {
    draw_pacman(msxCoord, msyCoord, mscurMove);
    if (mscurMove == 'r') {
        gl_draw_pixel(x+3, y+1, GL_HOTPINK);
        gl_draw_pixel(x+3, y+2, GL_HOTPINK);
        gl_draw_pixel(x+4, y+2, GL_HOTPINK);
        gl_draw_pixel(x+1, y+3, GL_HOTPINK);
        gl_draw_pixel(x+2, y+3, GL_HOTPINK);
        gl_draw_pixel(x+2, y+4, GL_HOTPINK);
    } else if (mscurMove == 'l' || mscurMove == 'd') {
        gl_draw_pixel(x+pacman_width-3, y+1, GL_HOTPINK);
        gl_draw_pixel(x+pacman_width-3, y+2, GL_HOTPINK);
        gl_draw_pixel(x+pacman_width-4, y+2, GL_HOTPINK);
        gl_draw_pixel(x+pacman_width-1, y+3, GL_HOTPINK);
        gl_draw_pixel(x+pacman_width-2, y+3, GL_HOTPINK);
        gl_draw_pixel(x+pacman_width-2, y+4, GL_HOTPINK);
    }
    if (mscurMove == 'u') {
        gl_draw_pixel(x+3, y+pacman_width-1, GL_HOTPINK);
        gl_draw_pixel(x+3, y+pacman_width-2, GL_HOTPINK);
        gl_draw_pixel(x+4, y+pacman_width-2, GL_HOTPINK);
        gl_draw_pixel(x+1, y+pacman_width-3, GL_HOTPINK);
        gl_draw_pixel(x+2, y+pacman_width-3, GL_HOTPINK);
        gl_draw_pixel(x+2, y+pacman_width-4, GL_HOTPINK);
    }
}

void draw_pacman(int x, int y, unsigned char move) {
    gl_draw_circle(x, y, pacman_width, pacman_width, GL_YELLOW);
    if (move == 'r') gl_draw_triangle(x + pacman_width / 2 - 2, y + pacman_width / 2, x + pacman_width - 1, y + pacman_width / 2 - 3, x + pacman_width - 1, y + pacman_width / 2 + 3, GL_BLACK);
    if (move == 'l') gl_draw_triangle(x + pacman_width - 5, y + pacman_width / 2 + 1, x, y + pacman_width / 2 - 3, x, y + pacman_width / 2 + 4, GL_BLACK);
    if (move == 'u') gl_draw_triangle(x + pacman_width / 2, y + pacman_width / 2 + 2, x + pacman_width / 2 - 3, y, x + pacman_width / 2 + 3, y, GL_BLACK);
    if (move == 'd') gl_draw_triangle(x + pacman_width / 2, y + pacman_width / 2 - 2, x + pacman_width / 2 - 3, y + pacman_width - 1, x + pacman_width / 2 + 3, y + pacman_width - 1, GL_BLACK);
}

void erase_pacman(int x, int y) {
    gl_draw_rect(x, y, 2*8 - 2, 2*8 - 2, GL_BLACK);
}

void ms_pacman_init() {
    msxCoord = 14*8 - 8 + 1;
    msyCoord = 28*8 - 4 - 2*8 + 1;
    msghostHit = 0;
    mscurMove = 'l';
    msnextMove = 'l';
    draw_ms_pacman(xCoord, yCoord); 
}

void pacman_init() {
    pacman_keyboard_init();
    xCoord = 14*8 - 8 + 1;
    yCoord = 28*8 - 4 - 2*8 + 1;
    ghostHit = 0;
    curMove = 'r';
    nextMove = 'r';
    superDotBonus = 0;
    frightenedPointsNotAdded = 0;
    draw_pacman(xCoord, yCoord, curMove); 
}

unsigned char pacman_get_curMove() {
    return curMove;
}

unsigned char ms_pacman_get_curMove() {
    return mscurMove;
}

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
    if (color == GL_BLUE && direction == 'd' && gl_read_pixel(x, y + 1 + 2*8 - 2) == GL_GREEN) side_hit = 1;
    if (color == GL_BLUE && direction == 'd' && gl_read_pixel(x + 2*8 - 2, y + 1 + 2*8 - 2) == GL_GREEN) side_hit = 1;
    if (color == GL_BLUE && direction == 'd' && gl_read_pixel(x + 8 - 1, y + 1 + 2*8 - 2) == GL_GREEN) side_hit = 1;
    return side_hit;
}

static int check_for_regular_ghosts(int x, int y) {
        int hit = 0;
        if (check_sides(x, y, 'l', GL_RED)) hit = 1;
        if (check_sides(x, y, 'r', GL_RED)) hit = 1;
        if (check_sides(x, y, 'u', GL_RED)) hit = 1;
        if (check_sides(x, y, 'd', GL_RED)) hit = 1;
        if (check_sides(x, y, 'l', GL_MAGENTA)) hit = 1;
        if (check_sides(x, y, 'r', GL_MAGENTA)) hit = 1;
        if (check_sides(x, y, 'u', GL_MAGENTA)) hit = 1;
        if (check_sides(x, y, 'd', GL_MAGENTA)) hit = 1;
        if (check_sides(x, y, 'l', GL_CYAN)) hit = 1;
        if (check_sides(x, y, 'r', GL_CYAN)) hit = 1;
        if (check_sides(x, y, 'u', GL_CYAN)) hit = 1;
        if (check_sides(x, y, 'd', GL_CYAN)) hit = 1;
        if (check_sides(x, y, 'l', GL_AMBER)) hit = 1;
        if (check_sides(x, y, 'r', GL_AMBER)) hit = 1;
        if (check_sides(x, y, 'u', GL_AMBER)) hit = 1;
        if (check_sides(x, y, 'd', GL_AMBER)) hit = 1;
        return hit;
}

static int check_for_frightened_ghosts(int x, int y) {
        if (check_sides(x, y, 'l', GL_PURPLE1) || check_sides(x, y, 'r', GL_PURPLE1) || check_sides(x, y, 'u', GL_PURPLE1) || check_sides(x, y, 'd', GL_PURPLE1)) {
            blinky_caught = 1;
            frightenedPointsNotAdded = 1;
            frightened_points = frightened_points + 10;
            return 1;
        }
        if (check_sides(x, y, 'l', GL_PURPLE2) || check_sides(x, y, 'r', GL_PURPLE2) || check_sides(x, y, 'u', GL_PURPLE2) || check_sides(x, y, 'd', GL_PURPLE2)) {
            pinky_caught = 1;
            frightenedPointsNotAdded = 1;
            frightened_points = frightened_points + 10;
            return 1;
        }
        if (check_sides(x, y, 'l', GL_PURPLE3) || check_sides(x, y, 'r', GL_PURPLE3) || check_sides(x, y, 'u', GL_PURPLE3) || check_sides(x, y, 'd', GL_PURPLE3)) {
            inky_caught = 1;
            frightenedPointsNotAdded = 1;
            frightened_points = frightened_points + 10;
            return 1;
    }
    if (check_sides(x, y, 'l', GL_PURPLE4) || check_sides(x, y, 'r', GL_PURPLE4) || check_sides(x, y, 'u', GL_PURPLE4) || check_sides(x, y, 'd', GL_PURPLE4)) {
        clyde_caught = 1;
        frightenedPointsNotAdded = 1;
        frightened_points = frightened_points + 10;
        return 1;
    }
    return 0;
}

static void check_for_dots(int x, int y, unsigned char move) {
    if (check_sides(x, y, move, GL_WHITE)) numDots--;
    if (check_sides(x, y, 'l', GL_OFFWHITE) || check_sides(x, y, 'r', GL_OFFWHITE) || check_sides(x, y, 'u', GL_OFFWHITE) || check_sides(x, y, 'd', GL_OFFWHITE)) {
        blinky_caught = 0;
        pinky_caught = 0;
        inky_caught = 0;
        clyde_caught = 0;
        blinky_to_center = 1;
        pinky_to_center = 1;
        inky_to_center = 1;
        clyde_to_center = 1;
        frightened = 1;
        frightened_start = timer_get_ticks();
        superDotBonus++;
    }
}

static void user_set_direction_keyboard() {
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
}

static void user_set_direction_nes() {
    read_nes_controller();
    if (!left_button) {
        nextMove = 'l';
    } else if (!right_button) {
        nextMove = 'r';
    } else if (!up_button) {
        nextMove = 'u';
    } else if (!down_button) {
        nextMove = 'd';
    }
    read_nes_controller_two();
    if (!msleft_button) {
        msnextMove = 'l';
    } else if (!msright_button) {
        msnextMove = 'r';
    } else if (!msup_button) {
        msnextMove = 'u';
    } else if (!msdown_button) {
        msnextMove = 'd';
    }
}

static void mstry_to_make_next_move() {
    if (msnextMove == 'r' && !check_sides(msxCoord, msyCoord, 'r', GL_BLUE)) mscurMove = 'r';
    if (msnextMove == 'l' && !check_sides(msxCoord, msyCoord, 'l', GL_BLUE)) mscurMove = 'l';
    if (msnextMove == 'u' && !check_sides(msxCoord, msyCoord, 'u', GL_BLUE)) mscurMove = 'u';
    if (msnextMove == 'd' && !check_sides(msxCoord, msyCoord, 'd', GL_BLUE)) mscurMove = 'd';
}

static void msmake_current_move() {
    if (mscurMove == 'r' && !check_sides(msxCoord, msyCoord, 'r', GL_BLUE)) msxCoord = msxCoord + 1;
    if (mscurMove == 'l' && !check_sides(msxCoord, msyCoord, 'l', GL_BLUE)) msxCoord = msxCoord - 1;
    if (mscurMove == 'u' && !check_sides(msxCoord, msyCoord, 'u', GL_BLUE)) msyCoord = msyCoord - 1;
    if (mscurMove == 'd' && !check_sides(msxCoord, msyCoord, 'd', GL_BLUE)) msyCoord = msyCoord + 1;
}

void ms_pacman_move() {
    erase_pacman(msxCoord, msyCoord);
    user_set_direction_nes();
    for (int i = 0; i < 6; i++) {
        mstry_to_make_next_move();
        check_for_dots(msxCoord, msyCoord, mscurMove);
        msghostHit = check_for_regular_ghosts(msxCoord, msyCoord);
        if (msghostHit) break;
        if (!frightened) frightened_points = 10;
        if (check_for_frightened_ghosts(msxCoord, msyCoord)) break;
        msmake_current_move();
        if (msxCoord <= 0) msxCoord = gl_get_width() - 1;
        if (msxCoord >= gl_get_width()) msxCoord = 1;
    }
    draw_ms_pacman(msxCoord, msyCoord);
}


static void try_to_make_next_move() {
    if (nextMove == 'r' && !check_sides(xCoord, yCoord, 'r', GL_BLUE)) curMove = 'r';
    if (nextMove == 'l' && !check_sides(xCoord, yCoord, 'l', GL_BLUE)) curMove = 'l';
    if (nextMove == 'u' && !check_sides(xCoord, yCoord, 'u', GL_BLUE)) curMove = 'u';
    if (nextMove == 'd' && !check_sides(xCoord, yCoord, 'd', GL_BLUE)) curMove = 'd';
}

static void make_current_move() {
    if (curMove == 'r' && !check_sides(xCoord, yCoord, 'r', GL_BLUE)) xCoord = xCoord + 1;
    if (curMove == 'l' && !check_sides(xCoord, yCoord, 'l', GL_BLUE)) xCoord = xCoord - 1;
    if (curMove == 'u' && !check_sides(xCoord, yCoord, 'u', GL_BLUE)) yCoord = yCoord - 1;
    if (curMove == 'd' && !check_sides(xCoord, yCoord, 'd', GL_BLUE)) yCoord = yCoord + 1;
}

void pacman_move() {
    erase_pacman(xCoord, yCoord);
    user_set_direction_nes();
    for (int i = 0; i < 6; i++) {
        try_to_make_next_move();
        check_for_dots(xCoord, yCoord, curMove);
        ghostHit = check_for_regular_ghosts(xCoord, yCoord);
        if (ghostHit) break;
        if (!frightened) frightened_points = 10;
        if (check_for_frightened_ghosts(xCoord, yCoord)) break;
        make_current_move();
        if (xCoord <= 0) xCoord = gl_get_width() - 1;
        if (xCoord >= gl_get_width()) xCoord = 1;
    }
    draw_pacman(xCoord, yCoord, curMove);
}
