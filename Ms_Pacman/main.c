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

//
#define BOARD_WIDTH 224
#define BOARD_HEIGHT 288
#define GAME_OVER_DELAY 5
#define NUM_SUPER_DOTS 4
#define TOTAL_DOTS 244
#define START_DELAY 3
#define INIT_NUM_LIVES 3
#define EXTRA_LIFE_SCORE 1000
#define TILE_WIDTH 8
#define NUM_SCORE_CHAR 9

//
static int charWidth;
static int charHeight;

//
static int lives;
static int extraLife;

/*
*
*/
static void start_screen() {
    char* title = pacman_printf("MS. PACMAN");
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
*
*/
static void game_over() {
    char* gameOver = pacman_printf("GAME OVER");
    gl_draw_string(BOARD_WIDTH / 2 - charWidth * (strlen(gameOver) / 2), BOARD_HEIGHT / 2 - charHeight / 2, gameOver, GL_RED);
    gl_swap_buffer();
}

/*
*
*/
static int divide(int numTens, int divider) {
    for (int i = 0; i < NUM_SCORE_CHAR - 1 - numTens; i++) {
        divider = divider / 10;
    }
    return divider;
}

/*
*
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
*
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
*
*/
static void move_characters(int twoPlayer) {
    if(!pacman_hit_ghost()) pacman_move();
    if (twoPlayer && !ms_pacman_hit_ghost()) ms_pacman_move();
    blinky_move();
    pinky_move();
    inky_move();
    clyde_move();
}

/*
*
*/
static void display_lives() {
    gl_draw_rect(0, 34*TILE_WIDTH, gl_get_width(), 2*TILE_WIDTH, GL_BLACK);
    for (int i = 0; i < lives - 1; i++) {
        draw_pacman(i*(2*TILE_WIDTH) + 1, 34*TILE_WIDTH, 'r');
    }
    gl_swap_buffer();
}

/*
*
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
    gl_swap_buffer();
    return prevScore;
}

/*
*
*/
static int update_while_still_alive(int prevScore, int twoPlayer) {
    move_characters(twoPlayer);
    prevScore = update_score(prevScore);
    if(pacman_hit_ghost()) erase_pacman(pacman_get_x(), pacman_get_y());
    if(twoPlayer && ms_pacman_hit_ghost()) erase_pacman(ms_pacman_get_x(), ms_pacman_get_y());
    return prevScore;
} 

/*
*
*/
static int reset_between_lives(int prevScore, int twoPlayer) {
    erase_everything(twoPlayer);
    if (pacman_hit_ghost() && ms_pacman_hit_ghost()) lives--;
    prevScore = prevScore + superDotBonus;
    if (!(numDots - NUM_SUPER_DOTS)) {
        draw_dots();
        prevScore = prevScore + TOTAL_DOTS;
    }
    return prevScore;
}

/*
*
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
          while(numDots - NUM_SUPER_DOTS && !(pacman_hit_ghost() && ms_pacman_hit_ghost())) {
              prevScore = update_while_still_alive(prevScore, twoPlayer);
          }
          prevScore = reset_between_lives(prevScore, twoPlayer);
      }
      game_over();
      timer_delay(GAME_OVER_DELAY);
  }
}

