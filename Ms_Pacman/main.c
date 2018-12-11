#include "printf.h"
#include "uart.h"
#include "board.h"
#include "gl.h"
#include "gl_more.h"
#include "pacman_char.h"
#include "timer.h"
#include "ghosts.h"
#include "nes.h"
#include "strings.h"
#include "pacman_printf.h"

//These define the the different delays, score-keeping elements, and lives
#define GAME_OVER_DELAY 5
#define TOTAL_DOTS NUMBER_OF_DOTS
#define START_DELAY 3
#define INIT_NUM_LIVES 3
#define EXTRA_LIFE_SCORE 1500
#define NUM_SCORE_CHAR 9

//These define the width and height of the characters for the start screen
static int charWidth;
static int charHeight;

//These keep track of the number of lives and the extra lives
static int lives;
static int extraLife;

/*
* This function will display the start screen at the beginning of every game. This will show the different modes that the user can
* chooses (1 or 2 player), as well as the name and how to start the game.
*/
static void start_screen() {
    char* title = pacman_printf("PACMAN");
    gl_draw_string(BOARD_WIDTH / 2 - charWidth * (strlen(title) / 2), charHeight / 2, title, GL_YELLOW);
    char* by = pacman_printf("PROJECT BY:");
    gl_draw_string(BOARD_WIDTH / 2 - charWidth * (strlen(by) / 2), 3 * BOARD_HEIGHT / 4 + charHeight, by, GL_CYAN); 
    char* name = pacman_printf("BLAKE JONES");
    gl_draw_string(BOARD_WIDTH / 2 - charWidth * (strlen(name) / 2), 3 * BOARD_HEIGHT / 4 + 2 * charHeight + 2, name, GL_MAGENTA);
    char* pressStart = pacman_printf("PRESS START");
    gl_draw_string(BOARD_WIDTH / 2 - charWidth * (strlen(pressStart) / 2), BOARD_HEIGHT / 2 - 4 * charHeight, pressStart, GL_WHITE);
    char* onePlay = pacman_printf("A: One Player ");
    gl_draw_string(BOARD_WIDTH / 2 - charWidth * (strlen(onePlay) / 2), BOARD_HEIGHT / 2 - 2 * charHeight, onePlay, GL_RED);
    char* twoPlay = pacman_printf("B: Two Players");
    gl_draw_string(BOARD_WIDTH / 2 - charWidth * (strlen(twoPlay) / 2), BOARD_HEIGHT / 2 - charHeight / 2, twoPlay, GL_AMBER);
}

/*
* This will print the word game over once the final life is lost in the game.
*/
static void game_over() {
    char* gameOver = pacman_printf("GAME OVER");
    gl_draw_string(BOARD_WIDTH / 2 - charWidth * (strlen(gameOver) / 2), BOARD_HEIGHT / 2 - charHeight / 2, gameOver, GL_RED);
    gl_swap_buffer();
}

/*
* This function will allow for the score to be added to the different sections in the scoreboard
*/
static int divide(int numTens, int divider) {
    for (int i = 0; i < NUM_SCORE_CHAR - 1 - numTens; i++) {
        divider = divider / 10;
    }
    return divider;
}

/*
* This function will erase all the different characters in between rounds.
*/
static void erase_everything(int twoPlayer) {
    erase_blinky();
    erase_pinky();
    erase_inky();
    erase_clyde();
    erase_pacman(pacman_get_x(), pacman_get_y());
    if (twoPlayer) erase_pacman(ms_pacman_get_x(), ms_pacman_get_y());
}

/*
* This function will read the NES controller on the start screen and determine which mode the user selects. When the user changes modes, it will
* move a purple box next to the mode to indicate to the user the current mode. When the user presses start, wherever the purple box is will indicate
* the mode chosen.
*/
static int determine_player_mode(int twoPlayer) {
    read_nes_controller();
    if (!a_button) twoPlayer = 0;
    if (!b_button) twoPlayer = 1;
    if (!twoPlayer) {
        gl_draw_rect(0, BOARD_HEIGHT / 2 - TILE_WIDTH / 2, TILE_WIDTH, TILE_WIDTH, GL_BLACK);
        gl_draw_rect(0, BOARD_HEIGHT / 2 - 3 * charHeight / 2 - TILE_WIDTH / 2, TILE_WIDTH, TILE_WIDTH, GL_PURPLE1);
    } else {
        gl_draw_rect(0, BOARD_HEIGHT / 2 - 3 * charHeight / 2 - TILE_WIDTH / 2, TILE_WIDTH, TILE_WIDTH, GL_BLACK);
        gl_draw_rect(0, BOARD_HEIGHT / 2 - TILE_WIDTH / 2, TILE_WIDTH, TILE_WIDTH, GL_PURPLE1);
    }
    gl_swap_buffer();
    return twoPlayer;
}

/*
* This function is called to move all the characters to their next location per loop. The characters move a different number
* of pixels to define their speeds relative to one another.
*/
static void move_characters(int twoPlayer) {
    blinky_move();
    pinky_move();
    inky_move();
    clyde_move();
    if (twoPlayer && !ms_pacman_hit_ghost()) ms_pacman_move();
    if(!pacman_hit_ghost()) pacman_move();
    gl_swap_buffer();
}

/*
* This function displays the lives at the bottom of the screen for the user. These are displayed by the number of pacman characters
* drawn on the bottom of the screen.
*/
static void display_lives() {
    gl_draw_rect(0, 34*TILE_WIDTH, gl_get_width(), 2*TILE_WIDTH, GL_BLACK);
    for (int i = 0; i < lives - 1; i++) {
        draw_pacman(i*(2*TILE_WIDTH) + 1, 34*TILE_WIDTH, 'r');
    }
    gl_swap_buffer();
}

/*
* This function will update the score as the user moves around the screen. This includes adding in the bonuses that the user collects for
* eating frightened ghosts, the bonus for eating a super dot, and the number of dots eaten from the screen. If the EXTRA_LIFE_SCORE is hit, 
* then the user will recieve another life (This is much easier to get in two player mode--this is done on purpose. This allows the two player
* game to be extended longer much more easily so that players can cooperate much more easily, and it can be more fun). 
*/
static int update_score(int prevScore) {
    gl_draw_rect(TILE_WIDTH, 1, NUM_SCORE_CHAR*charWidth, charHeight, GL_BLACK);
    if (frightenedPointsNotAdded) {
        prevScore = prevScore + frightened_points;
        frightenedPointsNotAdded = 0;
    }
    int total_score = prevScore + TOTAL_DOTS - numDots + superDotBonus;
    for (int i = 0; i < NUM_SCORE_CHAR; i++) {
        gl_draw_char(i*charWidth + TILE_WIDTH, 1, divide(i, total_score) % 10 + '0', GL_WHITE);
    }
    if (total_score > extraLife * EXTRA_LIFE_SCORE) {
        lives++;
        extraLife++;
        display_lives();
    }
    return prevScore;
}

/*
* This function is called to update the screen while the pacman characters are still alive. This includes moving the characters and
* erasing the characters when they hit a ghost.
*/
static int update_while_still_alive(int prevScore, int twoPlayer) {
    move_characters(twoPlayer);
    prevScore = update_score(prevScore);
    move_characters(twoPlayer);
    if(pacman_hit_ghost()) erase_pacman(pacman_get_x(), pacman_get_y());
    if(twoPlayer && ms_pacman_hit_ghost()) erase_pacman(ms_pacman_get_x(), ms_pacman_get_y());
    return prevScore;
} 

/*
* This function will reset the screen after the user dies. It will delay a little to indicate to the user a death, and will then
* erase all the characters from the screen. If all the dots are eaten (rather than a death), this function will also draw all the dots
* back into the screen, and will update the previous score so that all bonuses can be saved from the previous game.
*/
static int reset_between_lives(int prevScore, int twoPlayer) {
    timer_delay(1);
    erase_everything(twoPlayer);
    if (pacman_hit_ghost() && ms_pacman_hit_ghost()) lives--;
    prevScore = prevScore + superDotBonus;
    if (numDots - NUM_SUPER_DOTS <= 0) {
        draw_dots();
        prevScore = prevScore + TOTAL_DOTS;
    }
    return prevScore;
}

/*
* This function runs the game. The game is in a constant loop that will initialize everything a bring up the screen for the user to select a mode. This is also
* in a loop that the function remains spinning in until the user hits the start button. The game will then start either a one or two player game, depending on 
* the choice of the user. Then, the game will enter into a loop for the number of rounds played. This is determined by the number of lives, although can be
* extended by finishing a round without losing a life. More lives can be gained throughout the game. In this loop, all the characters are initialized so that 
* each round begins in the same place. This then enters a while loop for the movement throughout a round. This loop will continue updating the characters and
* the score until the user runs into a ghost, or they finish eating all of the dots. The game is played with NES controllers.
*/
void main(void) 
{
  charWidth = gl_get_char_width();
  charHeight = gl_get_char_height();
  gl_init(BOARD_WIDTH, BOARD_HEIGHT, GL_DOUBLEBUFFER);
  while(1) {
      lives = INIT_NUM_LIVES;
      nes_init();
      gl_clear(GL_BLACK);
      start_screen();
      start_button = 1;
      int twoPlayer = 0;
      while (start_button) {
          twoPlayer = determine_player_mode(twoPlayer);
      }
      board_init();
      draw_dots();
      int prevScore = 0;
      extraLife = 1;
      while (lives) {
          pacman_init();
          if (twoPlayer) ms_pacman_init();
          ghosts_init();
          display_lives();
          timer_delay(START_DELAY);
          while(numDots - NUM_SUPER_DOTS > 0 && !(pacman_hit_ghost() && ms_pacman_hit_ghost())) {
              prevScore = update_while_still_alive(prevScore, twoPlayer);
          }
          prevScore = reset_between_lives(prevScore, twoPlayer);
      }
      game_over();
      timer_delay(GAME_OVER_DELAY);
  }
}

