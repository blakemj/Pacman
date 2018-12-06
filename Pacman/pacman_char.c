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

static int ghostHit;

int frightened_points;
int superDotBonus;
int frightenedPointsNotAdded;

static unsigned char nextMove;
static unsigned char curMove;

int pacman_hit_ghost() {
    return ghostHit;
}

int pacman_get_x() {
    return xCoord;
}

int pacman_get_y() {
    return yCoord;
}

void draw_pacman(int x, int y) {
    gl_draw_circle(x, y, pacman_width, pacman_width, GL_YELLOW);
    if (curMove == 'r') gl_draw_triangle(x + pacman_width / 2 - 2, y + pacman_width / 2, x + pacman_width - 1, y + pacman_width / 2 - 3, x + pacman_width - 1, y + pacman_width / 2 + 3, GL_BLACK);
    if (curMove == 'l') gl_draw_triangle(x + pacman_width - 5, y + pacman_width / 2 + 1, x, y + pacman_width / 2 - 3, x, y + pacman_width / 2 + 4, GL_BLACK);
    if (curMove == 'u') gl_draw_triangle(x + pacman_width / 2, y + pacman_width / 2 + 2, x + pacman_width / 2 - 3, y, x + pacman_width / 2 + 3, y, GL_BLACK);
    if (curMove == 'd') gl_draw_triangle(x + pacman_width / 2, y + pacman_width / 2 - 2, x + pacman_width / 2 - 3, y + pacman_width - 1, x + pacman_width / 2 + 3, y + pacman_width - 1, GL_BLACK);
}

void erase_pacman() {
    gl_draw_rect(xCoord, yCoord, 2*8 - 2, 2*8 - 2, GL_BLACK);
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
    draw_pacman(xCoord, yCoord); 
}

unsigned char pacman_get_curMove() {
    return curMove;
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

static void check_for_regular_ghosts() {
        if (check_sides(xCoord, yCoord, 'l', GL_RED)) ghostHit = 1;
        if (check_sides(xCoord, yCoord, 'r', GL_RED)) ghostHit = 1;
        if (check_sides(xCoord, yCoord, 'u', GL_RED)) ghostHit = 1;
        if (check_sides(xCoord, yCoord, 'd', GL_RED)) ghostHit = 1;
        if (check_sides(xCoord, yCoord, 'l', GL_MAGENTA)) ghostHit = 1;
        if (check_sides(xCoord, yCoord, 'r', GL_MAGENTA)) ghostHit = 1;
        if (check_sides(xCoord, yCoord, 'u', GL_MAGENTA)) ghostHit = 1;
        if (check_sides(xCoord, yCoord, 'd', GL_MAGENTA)) ghostHit = 1;
        if (check_sides(xCoord, yCoord, 'l', GL_CYAN)) ghostHit = 1;
        if (check_sides(xCoord, yCoord, 'r', GL_CYAN)) ghostHit = 1;
        if (check_sides(xCoord, yCoord, 'u', GL_CYAN)) ghostHit = 1;
        if (check_sides(xCoord, yCoord, 'd', GL_CYAN)) ghostHit = 1;
        if (check_sides(xCoord, yCoord, 'l', GL_AMBER)) ghostHit = 1;
        if (check_sides(xCoord, yCoord, 'r', GL_AMBER)) ghostHit = 1;
        if (check_sides(xCoord, yCoord, 'u', GL_AMBER)) ghostHit = 1;
        if (check_sides(xCoord, yCoord, 'd', GL_AMBER)) ghostHit = 1;
}

static int check_for_frightened_ghosts() {
        if (check_sides(xCoord, yCoord, 'l', GL_PURPLE1) || check_sides(xCoord, yCoord, 'r', GL_PURPLE1) || check_sides(xCoord, yCoord, 'u', GL_PURPLE1) || check_sides(xCoord, yCoord, 'd', GL_PURPLE1)) {
            blinky_caught = 1;
            frightenedPointsNotAdded = 1;
            frightened_points = frightened_points + 10;
            return 1;
        }
        if (check_sides(xCoord, yCoord, 'l', GL_PURPLE2) || check_sides(xCoord, yCoord, 'r', GL_PURPLE2) || check_sides(xCoord, yCoord, 'u', GL_PURPLE2) || check_sides(xCoord, yCoord, 'd', GL_PURPLE2)) {
            pinky_caught = 1;
            frightenedPointsNotAdded = 1;
            frightened_points = frightened_points + 10;
            return 1;
        }
        if (check_sides(xCoord, yCoord, 'l', GL_PURPLE3) || check_sides(xCoord, yCoord, 'r', GL_PURPLE3) || check_sides(xCoord, yCoord, 'u', GL_PURPLE3) || check_sides(xCoord, yCoord, 'd', GL_PURPLE3)) {
            inky_caught = 1;
            frightenedPointsNotAdded = 1;
            frightened_points = frightened_points + 10;
            return 1;
    }
    if (check_sides(xCoord, yCoord, 'l', GL_PURPLE4) || check_sides(xCoord, yCoord, 'r', GL_PURPLE4) || check_sides(xCoord, yCoord, 'u', GL_PURPLE4) || check_sides(xCoord, yCoord, 'd', GL_PURPLE4)) {
        clyde_caught = 1;
        frightenedPointsNotAdded = 1;
        frightened_points = frightened_points + 10;
        return 1;
    }
    return 0;
}

static void check_for_dots() {
    if (check_sides(xCoord, yCoord, curMove, GL_WHITE)) numDots--;
    if (check_sides(xCoord, yCoord, 'l', GL_OFFWHITE) || check_sides(xCoord, yCoord, 'r', GL_OFFWHITE) || check_sides(xCoord, yCoord, 'u', GL_OFFWHITE) || check_sides(xCoord, yCoord, 'd', GL_OFFWHITE)) {
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
    erase_pacman();
    user_set_direction_nes();
    for (int i = 0; i < 6; i++) {
        try_to_make_next_move();
        check_for_dots();
        check_for_regular_ghosts();
        if (ghostHit) break;
        if (!frightened) frightened_points = 10;
        if (check_for_frightened_ghosts()) break;
        make_current_move();
        if (xCoord <= 0) xCoord = gl_get_width() - 1;
        if (xCoord >= gl_get_width()) xCoord = 1;
    }
    draw_pacman(xCoord, yCoord);
}
