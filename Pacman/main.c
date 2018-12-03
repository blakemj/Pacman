#include "printf.h"
#include "uart.h"
#include "board.h"
#include "gl.h"
#include "pacman_char.h"
#include "timer.h"
#include "ghosts.h"

static int divide(int numTens, int divider) {
    for (int i = 0; i < 8 - numTens; i++) {
        divider = divider / 10;
    }
    return divider;
}

void main(void) 
{
  int charWidth = gl_get_char_width();
  int charHeight = gl_get_char_height();
  int lives = 3;
  board_init();
  draw_dots();
  while (lives) {
      pacman_init();
      ghosts_init();
      gl_swap_buffer();
      timer_delay(3);
      while(numDots - 4 && !pacman_hit_ghost()) {
          pacman_move();
          blinky_move();
          pinky_move();
          inky_move();
          clyde_move();
          for (int i = 0; i < 9; i++) {
              gl_draw_char(i*charWidth + 8, 1, divide(i, 244 - numDots) % 10 + 48, GL_WHITE);
          }
          gl_swap_buffer();
          gl_draw_rect(8, 1, 9*charWidth, charHeight, GL_BLACK);
      }
      erase_blinky();
      erase_pinky();
      erase_inky();
      erase_clyde();
      erase_pacman();
      if (pacman_hit_ghost()) lives--;
      if (!(numDots - 4)) draw_dots();
  }
}
