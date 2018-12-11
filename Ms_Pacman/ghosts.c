#include "ghosts.h"
#include "gl.h"
#include "pacman_char.h"
#include "timer.h"
#include "board.h"
#include "gl_more.h"

//This defines different lengths and widths for the ghosts, the map, and the tiles
#define GHOST_WIDTH (2*TILE_WIDTH - 2)
#define MAP_LENGTH 36*TILE_WIDTH
#define MAP_WIDTH 28*TILE_WIDTH
#define CENTERX (14*8 - 8 + 1)
#define CENTERY (17*8 - 4 + 1)

//This defines the length of time frightened mode is on
#define FRIGHTENED_MODE_LENGTH 5

//This defines the length of time that scatter time is initially on, target time is on, and how many mode switches
//there are until it remains on target time 
#define INITIAL_SCATTER_TIME 12
#define TARGET_TIME 20
#define MAX_MODE_SWITCHES 4

//This defines the number of dots
#define TOTAL_NUM_DOTS NUMBER_OF_DOTS

//This defines the delay times for the ghosts to remain inititally in the ghost prison
#define PINKY_INITIAL_DELAY 4
#define INKY_INITIAL_DELAY 20
#define CLYDE_INITIAL_DELAY (TOTAL_NUM_DOTS / 3 - 15)

//These variables define the length of the full and half tiles
static int half_tile;
static int full_tile;

//These variables keep track of the colors of the ghosts when scared (changes when blinking)
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

//This variable keeps track of whether or not the ghosts are stuck in the ghost box/Should be sent there
static int blinkyStuckInBox;
static int pinkyStuckInBox;
static int inkyStuckInBox;
static int clydeStuckInBox;

//These keep track of the rectange that is below the ghost's position
static color_t saveRectBlinky[GHOST_WIDTH][GHOST_WIDTH];
static color_t saveRectPinky[GHOST_WIDTH][GHOST_WIDTH];
static color_t saveRectInky[GHOST_WIDTH][GHOST_WIDTH];
static color_t saveRectClyde[GHOST_WIDTH][GHOST_WIDTH];

//These variables change according to whether the ghost should be sent to the center
int blinky_to_center;
int pinky_to_center;
int inky_to_center;
int clyde_to_center;

//These variables keep track of the mode, the mode start times, and how often the modes have switched
static int mode_start;
static int scatter;
int frightened;
int frightened_start;
static int modeTime;
static int modeRound;

/*
* This function will draw the ghost shape. This will draw the circle for the head, will use a rectangle that covers half the head 
* for the body, will draw white rectangles for the eye whites, as well as two rectangles to give them a jagged bottom.
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
* This function will draw the ghost shape in the given ghost's color, then will draw pupils in. Blinky's pupils face the bottom left.
*/
static void draw_blinky_rect(color_t color) {
    draw_ghost_shape(blinky_x, blinky_y, color);
    gl_draw_rect(blinky_x + 2, blinky_y + 5, 2, 2, GL_BLUE);
    gl_draw_rect(blinky_x + 8, blinky_y + 5, 2, 2, GL_BLUE);
}

/*
* This function will draw the ghost shape in the given ghost's color, then will draw pupils in. Pinky's pupils face the bottom right.
*/
static void draw_pinky_rect(color_t color) {
    draw_ghost_shape(pinky_x, pinky_y, color);
    gl_draw_rect(pinky_x + 4, pinky_y + 5, 2, 2, GL_BLUE);
    gl_draw_rect(pinky_x + 10, pinky_y + 5, 2, 2, GL_BLUE);
}

/*
* This function will draw the ghost shape in the given ghost's color, then will draw pupils in. Inky's pupils face the top left.
*/
static void draw_inky_rect(color_t color) {
    draw_ghost_shape(inky_x, inky_y, color);
    gl_draw_rect(inky_x + 2, inky_y + 3, 2, 2, GL_BLUE);
    gl_draw_rect(inky_x + 8, inky_y + 3, 2, 2, GL_BLUE);
}

/*
* This function will draw the ghost shape in the given ghost's color, then will draw pupils in. Clyde's pupils face the top right.
*/
static void draw_clyde_rect(color_t color) {
    draw_ghost_shape(clyde_x, clyde_y, color);
    gl_draw_rect(clyde_x + 4, clyde_y + 3, 2, 2, GL_BLUE);
    gl_draw_rect(clyde_x + 10, clyde_y + 3, 2, 2, GL_BLUE);
}

/*
* This function will take save the rectangle that goes behind the ghost character in a temporary variable. This ensures that none of
* the dots are erased when the ghosts move around.
*/
static void save_under_ghost(int x, int y, color_t saveRect[GHOST_WIDTH][GHOST_WIDTH]) {
    for (int i = 0; i < GHOST_WIDTH; i++) {
        for (int j = 0; j < GHOST_WIDTH; j++) {
             saveRect[i][j] = gl_read_pixel(x + i, y + j);
        }
    }
}

/*
* This will rewrite the saved rectangle that was under the ghost to give the appearance of the ghost being erased
*/
void erase_ghost(int x, int y, color_t saveRect[GHOST_WIDTH][GHOST_WIDTH]) {
    for (int i = 0; i < GHOST_WIDTH; i++) {
        for (int j = 0; j < GHOST_WIDTH; j++) {
            gl_draw_pixel(x + i, y + j, saveRect[i][j]);
        }
    }
}

/*
* This erases the blinky ghost
*/
void erase_blinky() {
    erase_ghost(blinky_x, blinky_y, saveRectBlinky);
}

/*
* This erases the pinky ghost
*/
void erase_pinky() {
    erase_ghost(pinky_x, pinky_y, saveRectPinky);
}

/*
* This erases the inky ghost
*/
void erase_inky() {
    erase_ghost(inky_x, inky_y, saveRectInky);
}

/*
* This erases the clyde ghost
*/
void erase_clyde() {
    erase_ghost(clyde_x, clyde_y, saveRectClyde);
}

/*
* This function will save all of the rectangles from behind the ghosts, and will then
* draw the ghost on the screen for the color of the given mode
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
* This will change the scared color for each of the ghosts from their scared
* color to the alternate color, or the other way around
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
* This function will draw the ghosts in frightened mode for the frightened mode length. If there is one second left in frightened
* mode, then it will begin to flash the colors of the ghosts to signal a change back to regular mode. If another super dot is eaten
* then the color is reset to the original scared color and they will stop blinking.
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
* This function is called to switch the directions of the ghosts
*/
static unsigned char switch_directions(unsigned char curMove) {
    if (curMove == 'r') return 'l';
    if (curMove == 'l') return 'r';
    if (curMove == 'd') return 'u';
    if (curMove == 'u') return 'd';
    return '\0';
}

/*
* This function is called to determine when a mode change is needed. The modes will change from scatter to target mode, then from target to
* scatter mode four times before it just always remains in target mode for the rest of the round. The amount of time for scatter mode decreases
* in each mode round making it more and more difficult to get away from the ghosts.
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
* This function initializes blinky outside of the box.
*/
static void blinky_init() {
    blinky_x = CENTERX;
    blinky_y = CENTERY - 3*TILE_WIDTH;
    blinkyCurMove = 'l';
    blinkyStuckInBox = 5*TILE_WIDTH;
    blinky_caught = 0;
    blinky_to_center = 1;
}

/*
* This function initializes pinky in the center of the box
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
* This functon initializes inky in the left side of the box.
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
* This function initializes clyde in the right side of the box.
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
* This function will initialize the ghosts as well as the many variables that keep track of mode, location, and sizing
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
* This function is called to make a ghost make a decision. This function takes all the possible routes that a ghost can take (everywhere that isn't backwards
* or into a wall) and will check which one has the least distance to the target tile's x and y coordinates. The ghost will take the route that is the lowest 
* distance (even if this may end up making the overall distance to the target much longer). This function will return the move that is the ghost's new current
* move.
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
* This function is called to determine which pacman is the closest pacman between Ms. Pacman and Pacman. If in one-player mode, or if either pacman has
* died, the only pacman on the screen will simply be returned (1 meaning pacman, 0 meaning Ms. Pacman). The closest pacman is found by the stright-line
* distance, so even if a path may take less time, it will follow the stright-line closest pacman.
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
* This function is used to randomly decide a tile for the frightened ghost to follow. This is found by taking the distance to pacman, and modding
* with the number of tiles in the x and y direction respectively (respective to x and y distance from pacman). This gives back a somewhat random
* number which then is used to make the same ghost make a decision. This returns that decision.
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
* This function defines the movement of blinky. This will first check to see if blinky has been caught while in frightened mode. If so, this will
* send blinky to the center of the screen. Otherwise, the function will take the current direction of blinky and move them one pixel in that direction
* every time this function is called (unless they hit a dead end). Also, this function will determine if blinky has entered a warp tunnel to the opposite
* side of the screen, and will adjust accordingly.
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
* This function is called during target mode. In this function, blinky will directly target whichever pacman is closer. This sets blinky's 
* current move to whatever the path is where pacman's exact location is the target.
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
* This function will first erase blinky so that the new location can be updated. It is only when blinky is called where a possible change
* in mode can happen (since all of the ghosts are always on at the same time, this allows it to be called once per move). Then, depending on
* the mode, blinky's current move is changed so that blinky moves toward the current round's target. If blinky is stuck in the box, then for
* a few times, blinky will not have any movement (this delays the time it takes for blinky to come back out). Then, blinky's move will always
* be up so that blinky safely exits the box. This loops through 5 times, which sets blinky as the fastest ghost (since blinky moves 5 times
* per 'move'. Finally, blinky's move is actually made. (This all happens in the loop).
*/
void blinky_move() {
    erase_ghost(blinky_x, blinky_y, saveRectBlinky);
    for (int i = 0; i < 5; i++) {
        mode_change();
        if (blinkyStuckInBox < 3*TILE_WIDTH) { //This mode is for when blinky is stuck in the box
            blinkyStuckInBox++;
            if (blinkyStuckInBox < 0) return; 
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
* This function defines the movement of pinky. This will first check to see if pinky has been caught while in frightened mode. If so, this will
* send pinky to the center of the screen. Otherwise, the function will take the current direction of pinky and move them one pixel in that direction
* every time this function is called (unless they hit a dead end). Also, this function will determine if pinky has entered a warp tunnel to the opposite 
* side of the screen, and will adjust accordingly.
*/
static void pinky_movement() {
    //This moves pinky to the center and resets the movement variables
    if (pinky_caught && pinky_to_center) {
        pinky_x = CENTERX;
        pinky_y = CENTERY;
        pinky_to_center = 0;
        pinkyStuckInBox = -20;
        return;
    }
    //This moves the ghost according to their current direction
    if (pinkyCurMove == 'r' && !check_sides(pinky_x, pinky_y, 'r', GL_BLUE)) pinky_x = pinky_x + 1;
    if (pinkyCurMove == 'l' && !check_sides(pinky_x, pinky_y, 'l', GL_BLUE)) pinky_x = pinky_x - 1;
    if (pinkyCurMove == 'u' && !check_sides(pinky_x, pinky_y, 'u',GL_BLUE)) pinky_y = pinky_y - 1;
    if (pinkyCurMove == 'd' && !check_sides(pinky_x, pinky_y, 'd', GL_BLUE)) pinky_y = pinky_y + 1;
    //This loops the ghost through the warp tunnels
    if (pinky_x <= 0) pinky_x = gl_get_width() - 1;
    if (pinky_x >= gl_get_width()) pinky_x = 1;
}

/*
* This function is called during target mode. In this function, pinky will target the tile that is four 'tiles' ahead of whatever pacman
* is closest in that pacman's current direction. This target is then input into the ghost's decision, and the returned move is then set as
* pinky's current move.
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
* This function will move pinky according to the current mode. Before each move, pinky is erased so that pinky can be updated on the screen.
* Initially, pinky is stuck in the box for an initial delay amount of seconds. Then, if pinky is stuck in the box, pinky will only move up. 
* If pinky was caught, then similar to blinky, there will be some 'moves' where pinky cannot move at all, delaying the time pinky stays stuck. 
* After pinky's current move is determined, then pinky's movement will be updated. This happens in a loop four times making pinky of medium speed.
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
* This function defines the movement of inky. This will first check to see if inky has been caught while in frightened mode. If so, this will
* send inky to the center of the screen. Otherwise, the function will take the current direction of inky and move them one pixel in that direction
* every time this function is called (unless they hit a dead end). Also, this function will determine if inky has entered a warp tunnel to the opposite 
* side of the screen, and will adjust accordingly.
*/
static void inky_movement() {
    //This moves inky to the center and resets the movement variables
    if (inky_caught && inky_to_center) {
        inky_x = CENTERX;
        inky_y = CENTERY;
        inky_to_center = 0;
        inkyStuckInBox = -20;
        return;
    }
    //This moves the ghost according to their current direction
    if (inkyCurMove == 'r' && !check_sides(inky_x, inky_y, 'r', GL_BLUE)) inky_x = inky_x + 1;
    if (inkyCurMove == 'l' && !check_sides(inky_x, inky_y, 'l', GL_BLUE)) inky_x = inky_x - 1;
    if (inkyCurMove == 'u' && !check_sides(inky_x, inky_y, 'u',GL_BLUE)) inky_y = inky_y - 1;
    if (inkyCurMove == 'd' && !check_sides(inky_x, inky_y, 'd', GL_BLUE)) inky_y = inky_y + 1;
    //This loops the ghost through the warp tunnels
    if (inky_x <= 0) inky_x = gl_get_width() - 1;
    if (inky_x >= gl_get_width()) inky_x = 1;
}

/*
* This function is called during target mode. To find inky's target tile, first, the cloesest pacman is found. Then, the vector is taken from blinky
* to two spaces ahead of the closest pacman and then is doubled (This is a straight line vector). The end of this vector beomes the target for inky.
* This function will then make a decision on how to make it to this target, and then the current move is set to that.
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
* This function is used to determine inky's movement. First, inky is erased so that inky can be updated. Then, inky initially must wait a certain
* number of dots are eaten off the screen. Once this happens, inky will begin their movement to the right. Then, inky will move up to safely leave
* the box. If inky gets sent back to the box, inky will initially delay by not moving whatever its current move is, then it will skip to directly
* moving upwards out of the box. Otherwise, inky will make its move based on the target. Inky does this in a loop 4 times making inky a ghost of
* medium speed.
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
* This function defines the movement of clyde. This will first check to see if clyde has been caught while in frightened mode. If so, this will
* send clyde to the center of the screen. Otherwise, the function will take the current direction of clyde and move them one pixel in that direction
* every time this function is called (unless they hit a dead end). Also, this function will determine if clyde has entered a warp tunnel to the opposite 
* side of the screen, and will adjust accordingly.
*/
static void clyde_movement() {
    //This moves clyde to the center and resets the movement variables
    if (clyde_caught && clyde_to_center) {
        clyde_x = CENTERX;
        clyde_y = CENTERY;
        clyde_to_center = 0;
        clydeStuckInBox = -20;
        return;
    }
    //This moves the ghost according to their current direction
    if (clydeCurMove == 'r' && !check_sides(clyde_x, clyde_y, 'r', GL_BLUE)) clyde_x = clyde_x + 1;
    if (clydeCurMove == 'l' && !check_sides(clyde_x, clyde_y, 'l', GL_BLUE)) clyde_x = clyde_x - 1;
    if (clydeCurMove == 'u' && !check_sides(clyde_x, clyde_y, 'u',GL_BLUE)) clyde_y = clyde_y - 1;
    if (clydeCurMove == 'd' && !check_sides(clyde_x, clyde_y, 'd', GL_BLUE)) clyde_y = clyde_y + 1;
    //This loops the ghost through the warp tunnels
    if (clyde_x <= 0) clyde_x = gl_get_width() - 1;
    if (clyde_x >= gl_get_width()) clyde_x = 1;
}

/*
* Clyde's target is very similar to blinky's, except with one caveat. Clyde will target the closest pacman until clyde gets within 8 tile-lengths of
* that pacman. Then, clyde will target the corner they target during scatter mode. This make's clyde's movement very weird and unpredictable.
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
* This function is used to determine clyde's movement. First, clyde is erased so that inky can be updated. Then, clyde initially must wait a certain
* number of dots are eaten off the screen. Once this happens, clyde will begin their movement to the left. Then, clyde will move up to safely leave 
* the box. If clyde gets sent back to the box, clyde will initially delay by not moving whatever its current move is, then it will skip to directly
* moving upwards out of the box. Otherwise, clyde will make its move based on the target. Clyde does this in a loop 3 times making clyde a ghost of
* slowest speed. After all of this, clyde will call for all the ghosts to be drawn to the screen in their updated positions (This is only done during
* Clyde's move because clyde is the last ghost. The color that the ghosts are drawn in depends on whether or not frightened mode is on or not).
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
