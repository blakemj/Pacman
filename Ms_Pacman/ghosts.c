#include "ghosts.h"
#include "gl.h"
#include "pacman_char.h"
#include "timer.h"
#include "board.h"
#include "gl_more.h"

//
#define GHOST_WIDTH (2*TILE_WIDTH - 2)
#define MAP_LENGTH 36*TILE_WIDTH
#define MAP_WIDTH 28*TILE_WIDTH
#define CENTERX (14*8 - 8 + 1)
#define CENTERY (17*8 - 4 + 1)

//
#define FRIGHTENED_MODE_LENGTH 5

//
//#define BLINKY_SCARED GL_PURPLE1
//#define BLINKY_SCARED_ALT GL_HOTPINK1
//#define PINKY_SCARED GL_PURPLE2
//#define PINKY_SCARED_ALT GL_HOTPINK2
//#define INKY_SCARED GL_PURPLE3
//#define INKY_SCARED_ALT GL_HOTPINK3
//#define CLYDE_SCARED GL_PURPLE4
//#define CLYDE_SCARED_ALT GL_HOTPINK4

//
//#define BLINKY_REG_COLOR GL_RED
//#define PINKY_REG_COLOR GL_MAGENTA
//#define INKY_REG_COLOR GL_CYAN
//#define CLYDE_REG_COLOR GL_AMBER

//
#define INITIAL_SCATTER_TIME 12
#define TARGET_TIME 20
#define MAX_MODE_SWITCHES 4

//
#define TOTAL_NUM_DOTS 244

//
#define PINKY_INITIAL_DELAY 4
#define INKY_INITIAL_DELAY 20
#define CLYDE_INITIAL_DELAY (TOTAL_NUM_DOTS / 3 - 15)

//
static int half_tile;
static int full_tile;

//
static color_t blinky_scared_color = BLINKY_SCARED;
static color_t pinky_scared_color = PINKY_SCARED;
static color_t inky_scared_color = INKY_SCARED;
static color_t clyde_scared_color = CLYDE_SCARED;

//These are the coordinates (x, y) for all the ghosts
static int blinky_x;
static int blinky_y;
static int pinky_x;
static int pinky_y;
static int inky_x;
static int inky_y;
static int clyde_x;
static int clyde_y;

//This keeps track of the ghost's current direction
static unsigned char blinkyCurMove;
static unsigned char pinkyCurMove;
static unsigned char inkyCurMove;
static unsigned char clydeCurMove;

//
static int blinkyStuckInBox;
static int pinkyStuckInBox;
static int inkyStuckInBox;
static int clydeStuckInBox;

//These keep track of the rectange that is below the ghost's position
static color_t saveRectBlinky[GHOST_WIDTH][GHOST_WIDTH];
static color_t saveRectPinky[GHOST_WIDTH][GHOST_WIDTH];
static color_t saveRectInky[GHOST_WIDTH][GHOST_WIDTH];
static color_t saveRectClyde[GHOST_WIDTH][GHOST_WIDTH];

//
int blinky_to_center;
int pinky_to_center;
int inky_to_center;
int clyde_to_center;

//
static int mode_start;
static int scatter;
int frightened;
int frightened_start;
static int modeTime;
static int modeRound;

/*
*
*/
static void draw_ghost_shape(int x, int y, color_t color) {
    gl_draw_circle(x, y, (GHOST_WIDTH), (GHOST_WIDTH), color);
    gl_draw_rect(x, y + (GHOST_WIDTH) / 2 - 1, (GHOST_WIDTH), (GHOST_WIDTH) / 2 + 1, color);
    gl_draw_rect(x + 2, y + 3, half_tile, half_tile, GL_WHITE);
    gl_draw_rect(x + 8, y + 3, half_tile, half_tile, GL_WHITE);
    gl_draw_triangle(x + 1, y - 1 + GHOST_WIDTH, x + 1 + 2, y - 1 + GHOST_WIDTH - 3, x + 1 + half_tile, y - 1 + GHOST_WIDTH, GL_BLACK); 
    gl_draw_triangle(x + GHOST_WIDTH / 2 + 1, y - 1 + GHOST_WIDTH, x + GHOST_WIDTH / 2 + 1 + 2, y - 1 + GHOST_WIDTH - 3, x + GHOST_WIDTH / 2+ 1 + half_tile, y - 1 + GHOST_WIDTH, GL_BLACK);
}

/*
*
*/
static void draw_blinky_rect(color_t color) {
    draw_ghost_shape(blinky_x, blinky_y, color);
    gl_draw_rect(blinky_x + 2, blinky_y + 5, 2, 2, GL_BLUE);
    gl_draw_rect(blinky_x + 8, blinky_y + 5, 2, 2, GL_BLUE);
}

/*
*
*/
static void draw_pinky_rect(color_t color) {
    draw_ghost_shape(pinky_x, pinky_y, color);
    gl_draw_rect(pinky_x + 4, pinky_y + 5, 2, 2, GL_BLUE);
    gl_draw_rect(pinky_x + 10, pinky_y + 5, 2, 2, GL_BLUE);
}

/*
*
*/
static void draw_inky_rect(color_t color) {
    draw_ghost_shape(inky_x, inky_y, color);
    gl_draw_rect(inky_x + 2, inky_y + 3, 2, 2, GL_BLUE);
    gl_draw_rect(inky_x + 8, inky_y + 3, 2, 2, GL_BLUE);
}

/*
*
*/
static void draw_clyde_rect(color_t color) {
    draw_ghost_shape(clyde_x, clyde_y, color);
    gl_draw_rect(clyde_x + 4, clyde_y + 3, 2, 2, GL_BLUE);
    gl_draw_rect(clyde_x + 10, clyde_y + 3, 2, 2, GL_BLUE);
}

/*
*
*/
static void save_under_ghost(int x, int y, color_t saveRect[GHOST_WIDTH][GHOST_WIDTH]) {
    for (int i = 0; i < GHOST_WIDTH; i++) {
        for (int j = 0; j < GHOST_WIDTH; j++) {
             saveRect[i][j] = gl_read_pixel(x + i, y + j);
        }
    }
}

/*
*
*/
void erase_ghost(int x, int y, color_t saveRect[GHOST_WIDTH][GHOST_WIDTH]) {
    for (int i = 0; i < GHOST_WIDTH; i++) {
        for (int j = 0; j < GHOST_WIDTH; j++) {
            gl_draw_pixel(x + i, y + j, saveRect[i][j]);
        }
    }
}

/*
*
*/
void erase_blinky() {
    erase_ghost(blinky_x, blinky_y, saveRectBlinky);
}

/*
*
*/
void erase_pinky() {
    erase_ghost(pinky_x, pinky_y, saveRectPinky);
}

/*
*
*/
void erase_inky() {
    erase_ghost(inky_x, inky_y, saveRectInky);
}

/*
*
*/
void erase_clyde() {
    erase_ghost(clyde_x, clyde_y, saveRectClyde);
}

/*
*
*/
static void draw_ghosts(int mode) {
    save_under_ghost(clyde_x, clyde_y, saveRectClyde);
    save_under_ghost(inky_x, inky_y, saveRectInky);
    save_under_ghost(pinky_x, pinky_y, saveRectPinky);
    save_under_ghost(blinky_x, blinky_y, saveRectBlinky);
    if (mode && !blinky_caught) {
        draw_blinky_rect(blinky_scared_color);
    } else {
        draw_blinky_rect(BLINKY_REG_COLOR);
    }
    if (mode && !pinky_caught) {
        draw_pinky_rect(pinky_scared_color);
    } else {
        draw_pinky_rect(PINKY_REG_COLOR);
    }
    if (mode && !inky_caught) {
        draw_inky_rect(inky_scared_color);
    } else {
        draw_inky_rect(INKY_REG_COLOR);
    }
    if (mode && !clyde_caught) {
        draw_clyde_rect(clyde_scared_color);
    } else {
        draw_clyde_rect(CLYDE_REG_COLOR);
    }
}

/*
*
*/
static void blink_colors() {
    if (blinky_scared_color == BLINKY_SCARED) {
            blinky_scared_color = BLINKY_SCARED_ALT;
    } else {
            blinky_scared_color = BLINKY_SCARED;
    }
    if (pinky_scared_color == PINKY_SCARED) {
            pinky_scared_color = PINKY_SCARED_ALT;
    } else {
            pinky_scared_color = PINKY_SCARED;
    }  
    if (inky_scared_color == INKY_SCARED) {
            inky_scared_color = INKY_SCARED_ALT;
    } else {
            inky_scared_color = INKY_SCARED;
    }  
    if (clyde_scared_color == CLYDE_SCARED) {
            clyde_scared_color = CLYDE_SCARED_ALT;
    } else {
            clyde_scared_color = CLYDE_SCARED;
    }  
}

/*
*
*/
static void frightened_mode() {
    if (timer_get_ticks() - frightened_start < FRIGHTENED_MODE_LENGTH * 1000000) {
        if (timer_get_ticks() - frightened_start > (FRIGHTENED_MODE_LENGTH - 1) * 1000000) {
            blink_colors();
        } else {
            //This resets the color should a second super dot be eaten while ghosts are flashing
            blinky_scared_color = BLINKY_SCARED;
            pinky_scared_color = PINKY_SCARED;
            inky_scared_color = INKY_SCARED;
            clyde_scared_color = CLYDE_SCARED;
        }
        draw_ghosts(1);
    } else {
        //This resets everything back to normal
        frightened = 0;
        blinky_caught = 0;
        pinky_caught = 0;
        inky_caught = 0;
        clyde_caught = 0;
        blinky_to_center = 1;
        pinky_to_center = 1;
        inky_to_center = 1;
        clyde_to_center = 1;
        blinky_scared_color = BLINKY_SCARED;
        pinky_scared_color = PINKY_SCARED;
        inky_scared_color = INKY_SCARED;
        clyde_scared_color = CLYDE_SCARED;
        draw_ghosts(0);
    }
}

/*
*
*/
static unsigned char switch_directions(unsigned char curMove) {
    if (curMove == 'r') return 'l';
    if (curMove == 'l') return 'r';
    if (curMove == 'd') return 'u';
    if (curMove == 'u') return 'd';
    return '\0';
}

/*
*
*/
static void mode_change() {
    if (modeRound == MAX_MODE_SWITCHES) {
        scatter = 0;
        return;
    }
    if (timer_get_ticks() - mode_start > modeTime * 1000000) {
        scatter = !scatter;
        blinkyCurMove = switch_directions(blinkyCurMove);
        pinkyCurMove = switch_directions(pinkyCurMove);
        inkyCurMove = switch_directions(inkyCurMove);
        clydeCurMove = switch_directions(clydeCurMove);
        if (modeTime < TARGET_TIME) {
            modeTime = TARGET_TIME;
            modeRound++;
        } else {
            modeTime = INITIAL_SCATTER_TIME - 3 * modeRound;
        }
        mode_start = timer_get_ticks();
    }
}

/*
*
*/
static void blinky_init() {
    blinky_x = CENTERX;
    blinky_y = CENTERY - 3*TILE_WIDTH;
    blinkyCurMove = 'l';
    blinkyStuckInBox = 40;
    blinky_caught = 0;
    blinky_to_center = 1;
}

/*
*
*/
static void pinky_init() {
    pinky_x = CENTERX;
    pinky_y = CENTERY;
    pinkyCurMove = 'u';
    pinkyStuckInBox = timer_get_ticks();
    pinky_caught = 0;
    pinky_to_center = 1;
}

/*
*
*/
static void inky_init() {
    inky_x = CENTERX - 2*TILE_WIDTH;
    inky_y = CENTERY;
    inkyCurMove = 'r';
    inkyStuckInBox = 0;
    inky_caught = 0;
    inky_to_center = 1;
}

/*
*
*/
static void clyde_init() {
    clyde_x = CENTERX + 2*TILE_WIDTH;
    clyde_y = CENTERY;
    clydeCurMove = 'l';
    clydeStuckInBox = 0;
    clyde_caught = 0;
    clyde_to_center = 1;
}

/*
*
*/
void ghosts_init() {
    full_tile = board_get_tile_width();
    half_tile = full_tile / 2;
    blinky_init();
    pinky_init();
    inky_init();
    clyde_init();
    draw_ghosts(0);
    //Initialize different ghost modes (scatter on for start of INITIAL_SCATTER_TIME sec, frightened off)
    scatter = 1;
    frightened = 0;
    modeTime = INITIAL_SCATTER_TIME;
    modeRound = 0;
    mode_start = timer_get_ticks();
}

/*
*
*/
static unsigned char ghost_decision(int ghostx, int ghosty, unsigned char curMove, int targetx, int targety) {
    int checkRight = 0;
    int checkLeft = 0;
    int checkUp = 0;
    int checkDown = 0;
    unsigned char dontGo;
    //Ghosts cannot go backwards
    if (curMove == 'r') dontGo = 'l';
    if (curMove == 'l') dontGo = 'r';
    if (curMove == 'd') dontGo = 'u';
    if (curMove == 'u') dontGo = 'd';
    if (dontGo != 'r' && !check_sides(ghostx, ghosty, 'r', GL_BLUE)) checkRight = 1;
    if (dontGo != 'l' && !check_sides(ghostx, ghosty, 'l', GL_BLUE)) checkLeft = 1;
    if (dontGo != 'u' && !check_sides(ghostx, ghosty, 'u',GL_BLUE)) checkUp = 1;
    if (dontGo != 'd' && !check_sides(ghostx, ghosty, 'd', GL_BLUE)) checkDown = 1;
    //Find the open tile that is the closest distance to the target tile
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

/*
*
*/
static int closest_pacman(int x, int y) {
    if (pacman_hit_ghost()) return 0;
    if (ms_pacman_hit_ghost()) return 1;
    int targetX = pacman_get_x();
    int targetY = pacman_get_y();
    int pacDistance = (targetX - x) * (targetX - x) + (targetY - y) * (targetY - y);
    targetX = ms_pacman_get_x();
    targetY = ms_pacman_get_y();
    int mspacDistance = (targetX - x) * (targetX - x) + (targetY - y) * (targetY - y);
    if (mspacDistance < pacDistance) {
        return 0;
    } else {
        return 1;
    }
}

/*
*
*/
static unsigned char ghost_frightened_movement(int x, int y, unsigned char curMove) {
    int targetX = pacman_get_x();
    int targetY = pacman_get_y();
    if (pacman_get_curMove() == 'r') targetX = targetX + 2*TILE_WIDTH;
    if (pacman_get_curMove() == 'l') targetX = targetX - 2*TILE_WIDTH;
    if (pacman_get_curMove() == 'd') targetY = targetY + 2*TILE_WIDTH;
    if (pacman_get_curMove() == 'u') targetY = targetY - 2*TILE_WIDTH;
    targetX = ((targetX - x) % MAP_WIDTH);
    targetY = ((targetY - y) % MAP_LENGTH);
    return ghost_decision(x, y, curMove, targetX, targetY);
}

/*
*
*/
static void blinky_movement() {
    //This moves blinky to the center and resets the movement variables
    if (blinky_caught && blinky_to_center) {
        blinky_x = CENTERX;
        blinky_y = CENTERY;
        blinky_to_center = 0;
        blinkyStuckInBox = -20;
        return;
    }
    //This moves the ghost according to their current direction
    if (blinkyCurMove == 'r' && !check_sides(blinky_x, blinky_y, 'r', GL_BLUE)) blinky_x = blinky_x + 1;
    if (blinkyCurMove == 'l' && !check_sides(blinky_x, blinky_y, 'l', GL_BLUE)) blinky_x = blinky_x - 1;
    if (blinkyCurMove == 'u' && !check_sides(blinky_x, blinky_y, 'u',GL_BLUE)) blinky_y = blinky_y - 1;
    if (blinkyCurMove == 'd' && !check_sides(blinky_x, blinky_y, 'd', GL_BLUE)) blinky_y = blinky_y + 1;
    //This loops the ghost through the warp tunnels
    if (blinky_x <= 0) blinky_x = gl_get_width() - 1;
    if (blinky_x >= gl_get_width()) blinky_x = 1;
}

/*
*
*/
static void blinky_target_mode() {
    int isPacmanCloser = closest_pacman(blinky_x, blinky_y);
    if (isPacmanCloser) {
        blinkyCurMove = ghost_decision(blinky_x, blinky_y, blinkyCurMove, pacman_get_x(), pacman_get_y());
    } else {
        blinkyCurMove = ghost_decision(blinky_x, blinky_y, blinkyCurMove, ms_pacman_get_x(), ms_pacman_get_y());
    }
}

/*
*
*/
void blinky_move() {
    erase_ghost(blinky_x, blinky_y, saveRectBlinky);
    for (int i = 0; i < 5; i++) {
        mode_change();
        if (blinkyStuckInBox < 3*TILE_WIDTH) { //This mode is for when blinky is stuck in the box
            blinkyStuckInBox++;
            if (blinkyStuckInBox <= 0) return; 
            blinkyCurMove = 'u';
        } else if (frightened && !blinky_caught) { //This mode is movement during frightened mode
            blinkyCurMove = ghost_frightened_movement(blinky_x, blinky_y, blinkyCurMove);
        } else if (scatter) { //This mode is movement during scatter mode
            blinkyCurMove = ghost_decision(blinky_x, blinky_y, blinkyCurMove, 25*TILE_WIDTH, 0);
        } else { //This mode is movement during target mode
            blinky_target_mode();
        }
        blinky_movement();
    }
}

/*
*
*/
static void pinky_movement() {
    //
    if (pinky_caught && pinky_to_center) {
        pinky_x = CENTERX;
        pinky_y = CENTERY;
        pinky_to_center = 0;
        pinkyStuckInBox = -20;
        return;
    }
    //
    if (pinkyCurMove == 'r' && !check_sides(pinky_x, pinky_y, 'r', GL_BLUE)) pinky_x = pinky_x + 1;
    if (pinkyCurMove == 'l' && !check_sides(pinky_x, pinky_y, 'l', GL_BLUE)) pinky_x = pinky_x - 1;
    if (pinkyCurMove == 'u' && !check_sides(pinky_x, pinky_y, 'u',GL_BLUE)) pinky_y = pinky_y - 1;
    if (pinkyCurMove == 'd' && !check_sides(pinky_x, pinky_y, 'd', GL_BLUE)) pinky_y = pinky_y + 1;
    //
    if (pinky_x <= 0) pinky_x = gl_get_width() - 1;
    if (pinky_x >= gl_get_width()) pinky_x = 1;
}

/*
*
*/
static void pinky_target_mode() {
    int isPacmanCloser = closest_pacman(pinky_x, pinky_y);
    int targetX = pacman_get_x();
    int targetY = pacman_get_y();
    if (!isPacmanCloser) {
        targetX = ms_pacman_get_x();
        targetY = ms_pacman_get_y();
    }
    if (pacman_get_curMove() == 'r') targetX = targetX + 4*TILE_WIDTH;
    if (pacman_get_curMove() == 'l') targetX = targetX - 4*TILE_WIDTH;
    if (pacman_get_curMove() == 'd') targetY = targetY - 4*TILE_WIDTH;
    if (pacman_get_curMove() == 'u') targetY = targetY - 4*TILE_WIDTH;
    pinkyCurMove = ghost_decision(pinky_x, pinky_y, pinkyCurMove, targetX, targetY);
}

/*
*
*/
void pinky_move() {
    erase_ghost(pinky_x, pinky_y, saveRectPinky);
    if (timer_get_ticks() - pinkyStuckInBox < PINKY_INITIAL_DELAY * 1000000) return;
    for (int i = 0; i < 4; i++) {
        if (pinkyStuckInBox < 3*TILE_WIDTH) {
            pinkyStuckInBox++;
            if (pinkyStuckInBox < 0) return;
            pinkyCurMove = 'u';
        } else if (frightened && !pinky_caught) {
            pinkyCurMove = ghost_frightened_movement(pinky_x, pinky_y, pinkyCurMove);
        } else if (scatter) {
            pinkyCurMove = ghost_decision(pinky_x, pinky_y, pinkyCurMove, 2*TILE_WIDTH, 0);
        } else {
            pinky_target_mode();
        }
        pinky_movement();
    }
}

/*
*
*/
static void inky_movement() {
    //
    if (inky_caught && inky_to_center) {
        inky_x = CENTERX;
        inky_y = CENTERY;
        inky_to_center = 0;
        inkyStuckInBox = -20;
        return;
    }
    //
    if (inkyCurMove == 'r' && !check_sides(inky_x, inky_y, 'r', GL_BLUE)) inky_x = inky_x + 1;
    if (inkyCurMove == 'l' && !check_sides(inky_x, inky_y, 'l', GL_BLUE)) inky_x = inky_x - 1;
    if (inkyCurMove == 'u' && !check_sides(inky_x, inky_y, 'u',GL_BLUE)) inky_y = inky_y - 1;
    if (inkyCurMove == 'd' && !check_sides(inky_x, inky_y, 'd', GL_BLUE)) inky_y = inky_y + 1;
    //
    if (inky_x <= 0) inky_x = gl_get_width() - 1;
    if (inky_x >= gl_get_width()) inky_x = 1;
}

/*
*
*/
static void inky_target_mode() {
    int isPacmanCloser = closest_pacman(inky_x, inky_y);
    int targetX = pacman_get_x();
    int targetY = pacman_get_y();
    if (!isPacmanCloser) {
        targetX = ms_pacman_get_x();
        targetY = ms_pacman_get_y();
    }
    if (pacman_get_curMove() == 'r') targetX = targetX + 2*TILE_WIDTH;
    if (pacman_get_curMove() == 'l') targetX = targetX - 2*TILE_WIDTH;
    if (pacman_get_curMove() == 'd') targetY = targetY + 2*TILE_WIDTH;
    if (pacman_get_curMove() == 'u') targetY = targetY - 2*TILE_WIDTH;
    targetX = blinky_x + (targetX - blinky_x) * 2;
    targetY = blinky_y + (targetY - blinky_y) * 2;
    inkyCurMove = ghost_decision(inky_x, inky_y, inkyCurMove, targetX, targetY);
}

/*
*
*/
void inky_move() {
    erase_ghost(inky_x, inky_y, saveRectInky);
    if (TOTAL_NUM_DOTS - numDots < INKY_INITIAL_DELAY) return;
    for (int i = 0; i < 4; i++) {
        if (inkyStuckInBox < 2*TILE_WIDTH) {
            if (inkyStuckInBox == -1) {
                inkyStuckInBox = 2*TILE_WIDTH;
                return;
            }
            inkyStuckInBox++;
            if (inkyStuckInBox < 0) return;
            inkyCurMove = 'r';
        } else if (inkyStuckInBox < 5*TILE_WIDTH) {
            inkyCurMove = 'u';
            inkyStuckInBox++;
        } else if (frightened && !inky_caught) {
            inkyCurMove = ghost_frightened_movement(inky_x, inky_y, inkyCurMove);
        } else if (scatter) {
            inkyCurMove = ghost_decision(inky_x, inky_y, inkyCurMove, 27*8, 35*8);
        } else {
            inky_target_mode();
        }
        inky_movement();
    }
}

/*
*
*/
static void clyde_movement() {
    //
    if (clyde_caught && clyde_to_center) {
        clyde_x = CENTERX;
        clyde_y = CENTERY;
        clyde_to_center = 0;
        clydeStuckInBox = -20;
        return;
    }
    //
    if (clydeCurMove == 'r' && !check_sides(clyde_x, clyde_y, 'r', GL_BLUE)) clyde_x = clyde_x + 1;
    if (clydeCurMove == 'l' && !check_sides(clyde_x, clyde_y, 'l', GL_BLUE)) clyde_x = clyde_x - 1;
    if (clydeCurMove == 'u' && !check_sides(clyde_x, clyde_y, 'u',GL_BLUE)) clyde_y = clyde_y - 1;
    if (clydeCurMove == 'd' && !check_sides(clyde_x, clyde_y, 'd', GL_BLUE)) clyde_y = clyde_y + 1;
    //
    if (clyde_x <= 0) clyde_x = gl_get_width() - 1;
    if (clyde_x >= gl_get_width()) clyde_x = 1;
}

/*
*
*/
static void clyde_target_mode() {
    int isPacmanCloser = closest_pacman(clyde_x, clyde_y);
    int targetX = pacman_get_x();
    int targetY = pacman_get_y();
    if (!isPacmanCloser) {
        targetX = ms_pacman_get_x();
        targetY = ms_pacman_get_y();
    }   
    int distance = (targetX - clyde_x) * (targetX - clyde_x) + (targetY - clyde_y) * (targetY - clyde_y);
    if (distance > 8 * TILE_WIDTH) {
        clydeCurMove = ghost_decision(clyde_x, clyde_y, clydeCurMove, targetX, targetY);
    } else {
        clydeCurMove = ghost_decision(clyde_x, clyde_y, clydeCurMove, 0, 35*TILE_WIDTH);
    }
}

/*
*
*/
void clyde_move() {
    erase_ghost(clyde_x, clyde_y, saveRectClyde);
    if (!(TOTAL_NUM_DOTS - numDots < CLYDE_INITIAL_DELAY)) {
        for (int i = 0; i < 3; i++) {
            if (clydeStuckInBox < 2*TILE_WIDTH) {
                if (clydeStuckInBox == -1) {
                    clydeStuckInBox = 2*TILE_WIDTH;
                    break;
                }
                clydeStuckInBox++;
                if (clydeStuckInBox < 0) break;
                clydeCurMove = 'l';
            } else if (clydeStuckInBox < 5*TILE_WIDTH) {
                clydeCurMove = 'u';
                clydeStuckInBox++;
            } else if (frightened && !clyde_caught) { 
                clydeCurMove = ghost_frightened_movement(clyde_x, clyde_y, clydeCurMove);
            } else if(scatter) {
                clydeCurMove = ghost_decision(clyde_x, clyde_y, clydeCurMove, 0, 35*TILE_WIDTH);
            } else {
                clyde_target_mode();
            }
            clyde_movement();
        }
    }
    //Since clyde is the last ghost drawn, all the ghosts are drawn here
    if (frightened) {
        frightened_mode();
    } else {
        draw_ghosts(0);
    }
}
