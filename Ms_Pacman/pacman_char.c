#include "pacman_char.h"
#include "gl.h"
#include "timer.h"
#include "pacman_keyboard.h"
#include "gl_more.h"
#include "printf.h"
#include "board.h"
#include "ghosts.h"
#include "nes.h"

//This defines the width of pacman
#define pacman_width (2*TILE_WIDTH - 2)

//This defines where the center is (aka where the pacman characters start).
#define CENTERX (14*TILE_WIDTH - TILE_WIDTH + 1) 
#define CENTERY (28*TILE_WIDTH - 4 - 2*TILE_WIDTH + 1)

//This defines the initial value of the frightened points bonus per ghost
#define FRIGHTENED_POINTS_VALUE 10

//This keeps track of the x and y coordinates of the ghosts
static int xCoord;
static int yCoord;
static int msxCoord;
static int msyCoord;

//This keeps track of whether a ghost was hit by each pacman
static int ghostHit;
static int msghostHit = 1; //Starts at 1 so that one player does not rely on Ms. Pacman

//These keep track of the point bonuses and are used beyond this file
int frightened_points;
int superDotBonus;
int frightenedPointsNotAdded;

//These variables keep track of the next move asked by the user and the current move that the pacman character is making
static unsigned char nextMove;
static unsigned char curMove;
static unsigned char msnextMove;
static unsigned char mscurMove;

//This returns whether Ms. Pacman has hit a ghost
int ms_pacman_hit_ghost() {
    return msghostHit;
}

//This returns the x value of Ms. Pacman.
int ms_pacman_get_x() {
    return msxCoord;
}

//This returns the y value of Ms. Pacman
int ms_pacman_get_y() {
    return msyCoord;
}

//This returns whether pacman has hit a ghost
int pacman_hit_ghost() {
    return ghostHit;
}

//This returns the x value of pacman
int pacman_get_x() {
    return xCoord;
}

//This returns the y value of pacman
int pacman_get_y() {
    return yCoord;
}

/*
* This funciton is used to draw Ms. Pacman. Ms. Pacman is almost the same as drawing regular pacman, except she has a pink bow. This
* bow is drawn in the correct corner depending on Ms. Pacman's direction (since she turns according to her direction).
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
* This function draws a pacman character to the screen. This is done by drawing a yellow circle, with a triangle cut out from Pacman in the direction that
* the character is moving in. This creates a mouth. This is called by the main file to draw the lives as well
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

/*
* This funciton will initialize Ms. Pacman in the center of the screen and will set her current direction to left
*/
void ms_pacman_init() {
    msxCoord = CENTERX;
    msyCoord = CENTERY;
    msghostHit = 0;
    mscurMove = 'l';
    msnextMove = 'l';
    draw_ms_pacman(xCoord, yCoord); 
}

/*
* This funciton will initialize Pacman in the center of the screen and will set his current direction to right
*/
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

//This function will return pacman's current move
unsigned char pacman_get_curMove() {
    return curMove;
}

//This function will return Ms. Pacman's current move
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

/*
* This function will check all the sides of whichever pacman's x and y value is passed in for the current color of the ghost
*/
static int is_ghost_hit(int x, int y, color_t ghost_color) {
    if (check_sides(x, y, 'l', ghost_color)) return 1;
    if (check_sides(x, y, 'r', ghost_color)) return 1;
    if (check_sides(x, y, 'u', ghost_color)) return 1;
    if (check_sides(x, y, 'd', ghost_color)) return 1;
    return 0;
}

/*
* This function will check if a ghost has been hit by the pacman given by the x and y. This will check for all of the ghosts' regular
* colors during a regular, non-frightenend mode.
*/
static int check_for_regular_ghosts(int x, int y) {
    if (is_ghost_hit(x, y, BLINKY_REG_COLOR)) return 1;
    if (is_ghost_hit(x, y, PINKY_REG_COLOR)) return 1;
    if (is_ghost_hit(x, y, INKY_REG_COLOR)) return 1;
    if (is_ghost_hit(x, y, CLYDE_REG_COLOR)) return 1;
    return 0;
}

/*
* This function will check all the sides for a frightened ghost. This will check for both each of the ghosts' respecitve scared color, as well as their
* alternate that comes up when the ghost is flashing to signal the end of frightened mode. If any happen to be true, then the respective ghost will be
* marked as caught, and the frightened points bonus will increase by the value of the frightened points.
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
* This function will check the sides of the pacman character for dots. If any of the sides come back with the color of a dot, then the number of
* dots left will decrease by one. If any of the super dots are found, then the bonus will be added, and the ghosts will be set into frightened mode.
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

//This function will read the NES controller for player 1, and will update the asked for next move so that it is saved
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

//See nes_player_1
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

//This is called so that the user can use the nes controller to set the direction
static void user_set_direction_nes() {
    nes_player_1();
    nes_player_2();
}

/*
* This function will check the next move as asked for by the user, and will try all of the sides. This will continue to happen unless the next move
* asked for is changed (in which case it will repeat with that one), or until it is able to make the next move, in which case the current move will
* be set.
*/
static void mstry_to_make_next_move() {
    if (msnextMove == 'r' && !check_sides(msxCoord, msyCoord, 'r', GL_BLUE)) mscurMove = 'r';
    if (msnextMove == 'l' && !check_sides(msxCoord, msyCoord, 'l', GL_BLUE)) mscurMove = 'l';
    if (msnextMove == 'u' && !check_sides(msxCoord, msyCoord, 'u', GL_BLUE)) mscurMove = 'u';
    if (msnextMove == 'd' && !check_sides(msxCoord, msyCoord, 'd', GL_BLUE)) mscurMove = 'd';
}

/*
* This will check the side that the current move is in, and will move Ms. Pacman accordingly (moving one pixel in the direction)
*/
static void msmake_current_move() {
    if (mscurMove == 'r' && !check_sides(msxCoord, msyCoord, 'r', GL_BLUE)) msxCoord = msxCoord + 1;
    if (mscurMove == 'l' && !check_sides(msxCoord, msyCoord, 'l', GL_BLUE)) msxCoord = msxCoord - 1;
    if (mscurMove == 'u' && !check_sides(msxCoord, msyCoord, 'u', GL_BLUE)) msyCoord = msyCoord - 1;
    if (mscurMove == 'd' && !check_sides(msxCoord, msyCoord, 'd', GL_BLUE)) msyCoord = msyCoord + 1;
}

/*
* This function will move Ms. Pacman across the screen. First, her old position will be erased. Then, it will update the set direction by the
* user from the nes. Then, in a loop, it will try to make the next move that was just set. Then it will check for dots, check to see if a ghost
* has been hit (and will break if so), and will then make the current move. If the character enters a warp tunnel, the character will be redrawn on the
* other side of the screen. This all happens in a loop 6 times making the pacman characters the fastest in the game. Finally, after all movement is made
* the character is drawn to the screen.
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
* This function will check the next move as asked for by the user, and will try all of the sides. This will continue to happen unless the next move
* asked for is changed (in which case it will repeat with that one), or until it is able to make the next move, in which case the current move will
* be set.
*/
static void try_to_make_next_move() {
    if (nextMove == 'r' && !check_sides(xCoord, yCoord, 'r', GL_BLUE)) curMove = 'r';
    if (nextMove == 'l' && !check_sides(xCoord, yCoord, 'l', GL_BLUE)) curMove = 'l';
    if (nextMove == 'u' && !check_sides(xCoord, yCoord, 'u', GL_BLUE)) curMove = 'u';
    if (nextMove == 'd' && !check_sides(xCoord, yCoord, 'd', GL_BLUE)) curMove = 'd';
}

/*
* This will check the side that the current move is in, and will move Ms. Pacman accordingly (moving one pixel in the direction)
*/
static void make_current_move() {
    if (curMove == 'r' && !check_sides(xCoord, yCoord, 'r', GL_BLUE)) xCoord++;
    if (curMove == 'l' && !check_sides(xCoord, yCoord, 'l', GL_BLUE)) xCoord--;
    if (curMove == 'u' && !check_sides(xCoord, yCoord, 'u', GL_BLUE)) yCoord--;
    if (curMove == 'd' && !check_sides(xCoord, yCoord, 'd', GL_BLUE)) yCoord++;
}

/*
* This function will move Pacman across the screen. First, his old position will be erased. Then, it will update the set direction by the
* user from the nes. Then, in a loop, it will try to make the next move that was just set. Then it will check for dots, check to see if a ghost
* has been hit (and will break if so), and will then make the current move. If the character enters a warp tunnel, the character will be redrawn on the
* other side of the screen. This all happens in a loop 6 times making the pacman characters the fastest in the game. Finally, after all movement is made
* the character is drawn to the screen.
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
