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

#define BOARD_WIDTH 224
#define BOARD_HEIGHT 288

static int charWidth;
static int charHeight;

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

static void game_over() {
    char* gameOver = pacman_printf("GAME OVER");
    gl_draw_string(BOARD_WIDTH / 2 - charWidth * (strlen(gameOver) / 2), BOARD_HEIGHT / 2 - charHeight / 2, gameOver, GL_RED);
    gl_swap_buffer();
}

static int divide(int numTens, int divider) {
    for (int i = 0; i < 8 - numTens; i++) {
        divider = divider / 10;
    }
    return divider;
}

static int total_score;

void main(void) 
{
  charWidth = gl_get_char_width();
  charHeight = gl_get_char_height();
  gl_init(BOARD_WIDTH, BOARD_HEIGHT, GL_DOUBLEBUFFER);
  while(1) {
      int lives = 3;
      int frightenedBonus = 0;
      nes_init();
      gl_clear(GL_BLACK);
      start_screen();
      start_button = 1;
      int twoPlayer = 0;
      while (start_button) {
          read_nes_controller();
          if (!a_button) twoPlayer = 0;
          if (!b_button) twoPlayer = 1;
          if (!twoPlayer) {
              gl_draw_rect(0, BOARD_HEIGHT / 2 - 4, 8, 8, GL_BLACK);
              gl_draw_rect(0, BOARD_HEIGHT / 2 - 3 * charHeight / 2 - 4, 8, 8, GL_PURPLE1);
          } else {
              gl_draw_rect(0, BOARD_HEIGHT / 2 - 3 * charHeight / 2 - 4, 8, 8, GL_BLACK);
              gl_draw_rect(0, BOARD_HEIGHT / 2 - 4, 8, 8, GL_PURPLE1);
          }
          gl_swap_buffer();
      }
      board_init();
      draw_dots();
      int prevScore = 0;
      while (lives) {
          pacman_init();
          if (twoPlayer) ms_pacman_init();
          ghosts_init();
          for (int i = 0; i < lives - 1; i++) {
              draw_pacman(i*(2*8) + 1, 34*8, pacman_get_curMove());
          }
          gl_swap_buffer();
          timer_delay(3);
          while(numDots - 4 && !(pacman_hit_ghost() && ms_pacman_hit_ghost())) {
              if(!pacman_hit_ghost()) pacman_move();
              if (twoPlayer && !ms_pacman_hit_ghost()) ms_pacman_move();
              blinky_move();
              pinky_move();
              inky_move();
              clyde_move();
              if (frightenedPointsNotAdded) {
                  frightenedBonus = frightenedBonus + frightened_points;
                  frightenedPointsNotAdded = 0;
              }
              total_score = prevScore + 244 - numDots + superDotBonus + frightenedBonus;
              if (total_score > 10000) lives++;
              for (int i = 0; i < 9; i++) {
                  gl_draw_char(i*charWidth + 8, 1, divide(i, total_score) % 10 + 48, GL_WHITE);
              }
              gl_swap_buffer();
              gl_draw_rect(8, 1, 9*charWidth, charHeight, GL_BLACK);
              if(pacman_hit_ghost()) erase_pacman(pacman_get_x(), pacman_get_y());
              if(twoPlayer && ms_pacman_hit_ghost()) erase_pacman(ms_pacman_get_x(), ms_pacman_get_y());
          }
          erase_blinky();
          erase_pinky();
          erase_inky();
          erase_clyde();
          erase_pacman(pacman_get_x(), pacman_get_y());
          erase_pacman(ms_pacman_get_x(), ms_pacman_get_y());
          prevScore = total_score;
          if (pacman_hit_ghost() && ms_pacman_hit_ghost()) lives--;
          if (!(numDots - 4)) draw_dots();
          gl_draw_rect(0, 34*8, gl_get_width(), 2*8, GL_BLACK);
      }
      game_over();
      timer_delay(5);
  }
}

