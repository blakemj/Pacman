#include "pacman_char.h"
#include "gl.h"
#include "timer.h"
#include "pacman_keyboard.h"
#include "gl_more.h"
#include "printf.h"
#include "board.h"
#include "ghosts.h"
#include "nes.h"

//
#define pacman_width (2*TILE_WIDTH - 2)

#define CENTERX (14*TILE_WIDTH - TILE_WIDTH + 1) 
#define CENTERY (28*TILE_WIDTH - 4 - 2*TILE_WIDTH + 1)

#define FRIGHTENED_POINTS_VALUE 10
//
static int xCoord;
static int yCoord;
static int msxCoord;
static int msyCoord;

//
static int ghostHit;
static int msghostHit = 1; //Starts at 1 so that one player does not rely on Ms. Pacman

//
int frightened_points;
int superDotBonus;
int frightenedPointsNotAdded;

//
static unsigned char nextMove;
static unsigned char curMove;
static unsigned char msnextMove;
static unsigned char mscurMove;

//
int ms_pacman_hit_ghost() {
    return msghostHit;
}

//
int ms_pacman_get_x() {
    return msxCoord;
}

//
int ms_pacman_get_y() {
    return msyCoord;
}

//
int pacman_hit_ghost() {
    return ghostHit;
}

//
int pacman_get_x() {
    return xCoord;
}

//
int pacman_get_y() {
    return yCoord;
}

/*
*
*/
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

/*
*
*/
void draw_pacman(int x, int y, unsigned char move) {
    gl_draw_circle(x, y, pacman_width, pacman_width, GL_YELLOW);
    if (move == 'r') gl_draw_triangle(x + pacman_width / 2 - 2, y + pacman_width / 2, x + pacman_width - 1, y + pacman_width / 2 - 3, x + pacman_width - 1, y + pacman_width / 2 + 3, GL_BLACK);
    if (move == 'l') gl_draw_triangle(x + pacman_width - 5, y + pacman_width / 2 + 1, x, y + pacman_width / 2 - 3, x, y + pacman_width / 2 + 4, GL_BLACK);
    if (move == 'u') gl_draw_triangle(x + pacman_width / 2, y + pacman_width / 2 + 2, x + pacman_width / 2 - 3, y, x + pacman_width / 2 + 3, y, GL_BLACK);
    if (move == 'd') gl_draw_triangle(x + pacman_width / 2, y + pacman_width / 2 - 2, x + pacman_width / 2 - 3, y + pacman_width - 1, x + pacman_width / 2 + 3, y + pacman_width - 1, GL_BLACK);
}

// This will simply cover pacman with a black rectangle to match the background
void erase_pacman(int x, int y) {
    gl_draw_rect(x, y, pacman_width, pacman_width, GL_BLACK);
}

void ms_pacman_init() {
    msxCoord = CENTERX;
    msyCoord = CENTERY;
    msghostHit = 0;
    mscurMove = 'l';
    msnextMove = 'l';
    draw_ms_pacman(xCoord, yCoord); 
}

void pacman_init() {
// Add this if wishing to use the keyboard   pacman_keyboard_init(); 
    xCoord = CENTERX;
    yCoord = CENTERY;
    ghostHit = 0;
    curMove = 'r';
    nextMove = 'r';
    superDotBonus = 0;
    frightenedPointsNotAdded = 0;
    draw_pacman(xCoord, yCoord, curMove); 
}

//
unsigned char pacman_get_curMove() {
    return curMove;
}

//
unsigned char ms_pacman_get_curMove() {
    return mscurMove;
}

/*
* This will check the two corners and the center pixel of the side in the given direction for the given color.
* These corners and sides are based on the rectangle that surrounds pacman (of 2 tile widths in width and height).
* If the direction is down and the color searched for is the color of the board, it will also check for the green
* door that is incorporated in the ghost prison, since this is included in the borders of the maze. If a side would
* be hit, it returns one, else it will return 0.
*/
int check_sides(int x, int y, unsigned char direction, color_t color) {
    if (direction == 'r' && gl_read_pixel(x + 1 + pacman_width, y) == color) return 1;
    if (direction == 'r' && gl_read_pixel(x + 1 + pacman_width, y + pacman_width) == color) return 1;
    if (direction == 'r' && gl_read_pixel(x + 1 + pacman_width, y + pacman_width / 2) == color) return 1;
    if (direction == 'l' && gl_read_pixel(x - 1, y) == color) return 1;
    if (direction == 'l' && gl_read_pixel(x - 1, y + pacman_width) == color) return 1;
    if (direction == 'l' && gl_read_pixel(x - 1, y + pacman_width / 2) == color) return 1;
    if (direction == 'u' && gl_read_pixel(x, y - 1) == color) return 1;
    if (direction == 'u' && gl_read_pixel(x + pacman_width, y - 1) == color) return 1;
    if (direction == 'u' && gl_read_pixel(x + pacman_width / 2, y - 1) == color) return 1;
    if (direction == 'd' && gl_read_pixel(x, y + 1 + pacman_width) == color) return 1;
    if (direction == 'd' && gl_read_pixel(x + pacman_width, y + 1 + pacman_width) == color) return 1;
    if (direction == 'd' && gl_read_pixel(x + pacman_width / 2, y + 1 + pacman_width) == color) return 1;
    if (color == GL_BLUE && direction == 'd' && gl_read_pixel(x, y + 1 + pacman_width) == GL_GREEN) return 11;
    if (color == GL_BLUE && direction == 'd' && gl_read_pixel(x + pacman_width, y + 1 + pacman_width) == GL_GREEN) return 1;
    if (color == GL_BLUE && direction == 'd' && gl_read_pixel(x + pacman_width / 2, y + 1 + pacman_width) == GL_GREEN) return 1;
    return 0;
}

static int is_ghost_hit(int x, int y, color_t ghost_color) {
    if (check_sides(x, y, 'l', ghost_color)) return 1;
    if (check_sides(x, y, 'r', ghost_color)) return 1;
    if (check_sides(x, y, 'u', ghost_color)) return 1;
    if (check_sides(x, y, 'd', ghost_color)) return 1;
    return 0;
}

static int check_for_regular_ghosts(int x, int y) {
    if (is_ghost_hit(x, y, BLINKY_REG_COLOR)) return 1;
    if (is_ghost_hit(x, y, PINKY_REG_COLOR)) return 1;
    if (is_ghost_hit(x, y, INKY_REG_COLOR)) return 1;
    if (is_ghost_hit(x, y, CLYDE_REG_COLOR)) return 1;
    return 0;
}

/*
*
*/
static int check_for_frightened_ghosts(int x, int y) {
        if (check_sides(x, y, 'l', BLINKY_SCARED) || check_sides(x, y, 'r', BLINKY_SCARED) || check_sides(x, y, 'u', BLINKY_SCARED) || check_sides(x, y, 'd', BLINKY_SCARED) || check_sides(x, y, 'l', BLINKY_SCARED_ALT) || check_sides(x, y, 'r', BLINKY_SCARED_ALT) || check_sides(x, y, 'u', BLINKY_SCARED_ALT) || check_sides(x, y, 'd', BLINKY_SCARED_ALT)) {
            blinky_caught = 1;
            frightenedPointsNotAdded = 1;
            frightened_points = frightened_points + FRIGHTENED_POINTS_VALUE;
            return 1;
        }
        if (check_sides(x, y, 'l', PINKY_SCARED) || check_sides(x, y, 'r', PINKY_SCARED) || check_sides(x, y, 'u', PINKY_SCARED) || check_sides(x, y, 'd', PINKY_SCARED) || check_sides(x, y, 'l', PINKY_SCARED_ALT) || check_sides(x, y, 'r', PINKY_SCARED_ALT) || check_sides(x, y, 'u', PINKY_SCARED_ALT) || check_sides(x, y, 'd', PINKY_SCARED_ALT)) {
            pinky_caught = 1;
            frightenedPointsNotAdded = 1;
            frightened_points = frightened_points + FRIGHTENED_POINTS_VALUE;
            return 1;
        }
        if (check_sides(x, y, 'l', INKY_SCARED) || check_sides(x, y, 'r', INKY_SCARED) || check_sides(x, y, 'u', INKY_SCARED) || check_sides(x, y, 'd', INKY_SCARED) || check_sides(x, y, 'l', INKY_SCARED_ALT) || check_sides(x, y, 'r', INKY_SCARED_ALT) || check_sides(x, y, 'u', INKY_SCARED_ALT) || check_sides(x, y, 'd', INKY_SCARED_ALT)) {
            inky_caught = 1;
            frightenedPointsNotAdded = 1;
            frightened_points = frightened_points + FRIGHTENED_POINTS_VALUE;
            return 1;
    }
    if (check_sides(x, y, 'l', CLYDE_SCARED) || check_sides(x, y, 'r', CLYDE_SCARED) || check_sides(x, y, 'u', CLYDE_SCARED) || check_sides(x, y, 'd', CLYDE_SCARED) || check_sides(x, y, 'l', CLYDE_SCARED_ALT) || check_sides(x, y, 'r', CLYDE_SCARED_ALT) || check_sides(x, y, 'u', CLYDE_SCARED_ALT) || check_sides(x, y, 'd', CLYDE_SCARED_ALT)) {
        clyde_caught = 1;
        frightenedPointsNotAdded = 1;
        frightened_points = frightened_points + FRIGHTENED_POINTS_VALUE;
        return 1;
    }
    return 0;
}

/*
*
*/
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

/*
* Add this if you want to use the keyboard
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
*/

static void nes_player_1() {
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
}

static void nes_player_2() {
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

static void user_set_direction_nes() {
    nes_player_1();
    nes_player_2();
}

/*
*
*/
static void mstry_to_make_next_move() {
    if (msnextMove == 'r' && !check_sides(msxCoord, msyCoord, 'r', GL_BLUE)) mscurMove = 'r';
    if (msnextMove == 'l' && !check_sides(msxCoord, msyCoord, 'l', GL_BLUE)) mscurMove = 'l';
    if (msnextMove == 'u' && !check_sides(msxCoord, msyCoord, 'u', GL_BLUE)) mscurMove = 'u';
    if (msnextMove == 'd' && !check_sides(msxCoord, msyCoord, 'd', GL_BLUE)) mscurMove = 'd';
}

/*
*
*/
static void msmake_current_move() {
    if (mscurMove == 'r' && !check_sides(msxCoord, msyCoord, 'r', GL_BLUE)) msxCoord = msxCoord + 1;
    if (mscurMove == 'l' && !check_sides(msxCoord, msyCoord, 'l', GL_BLUE)) msxCoord = msxCoord - 1;
    if (mscurMove == 'u' && !check_sides(msxCoord, msyCoord, 'u', GL_BLUE)) msyCoord = msyCoord - 1;
    if (mscurMove == 'd' && !check_sides(msxCoord, msyCoord, 'd', GL_BLUE)) msyCoord = msyCoord + 1;
}

/*
*
*/
void ms_pacman_move() {
    erase_pacman(msxCoord, msyCoord);
    user_set_direction_nes();
    for (int i = 0; i < 6; i++) {
        mstry_to_make_next_move();
        check_for_dots(msxCoord, msyCoord, mscurMove);
        msghostHit = check_for_regular_ghosts(msxCoord, msyCoord);
        if (msghostHit) break;
        if (!frightened) frightened_points = FRIGHTENED_POINTS_VALUE;
        if (check_for_frightened_ghosts(msxCoord, msyCoord)) break;
        msmake_current_move();
        if (msxCoord <= 0) msxCoord = gl_get_width() - 1;
        if (msxCoord >= gl_get_width()) msxCoord = 1;
    }
    draw_ms_pacman(msxCoord, msyCoord);
}

/*
*
*/
static void try_to_make_next_move() {
    if (nextMove == 'r' && !check_sides(xCoord, yCoord, 'r', GL_BLUE)) curMove = 'r';
    if (nextMove == 'l' && !check_sides(xCoord, yCoord, 'l', GL_BLUE)) curMove = 'l';
    if (nextMove == 'u' && !check_sides(xCoord, yCoord, 'u', GL_BLUE)) curMove = 'u';
    if (nextMove == 'd' && !check_sides(xCoord, yCoord, 'd', GL_BLUE)) curMove = 'd';
}

/*
*
*/
static void make_current_move() {
    if (curMove == 'r' && !check_sides(xCoord, yCoord, 'r', GL_BLUE)) xCoord++;
    if (curMove == 'l' && !check_sides(xCoord, yCoord, 'l', GL_BLUE)) xCoord--;
    if (curMove == 'u' && !check_sides(xCoord, yCoord, 'u', GL_BLUE)) yCoord--;
    if (curMove == 'd' && !check_sides(xCoord, yCoord, 'd', GL_BLUE)) yCoord++;
}

/*
*
*/
void pacman_move() {
    erase_pacman(xCoord, yCoord);
    user_set_direction_nes();
    for (int i = 0; i < 6; i++) {
        try_to_make_next_move();
        check_for_dots(xCoord, yCoord, curMove);
        ghostHit = check_for_regular_ghosts(xCoord, yCoord);
        if (ghostHit) break;
        if (!frightened) frightened_points = FRIGHTENED_POINTS_VALUE;
        if (check_for_frightened_ghosts(xCoord, yCoord)) break;
        make_current_move();
        if (xCoord <= 0) xCoord = gl_get_width() - 1;
        if (xCoord >= gl_get_width()) xCoord = 1;
    }
    draw_pacman(xCoord, yCoord, curMove);
}
