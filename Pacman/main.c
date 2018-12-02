#include "printf.h"
#include "uart.h"
#include "board.h"
#include "gl.h"
#include "pacman_char.h"
#include "timer.h"
#include "ghosts.h"

void main(void) 
{
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
          gl_swap_buffer();
      }
      erase_blinky();
      erase_pinky();
      erase_pacman();
      if (pacman_hit_ghost()) lives--;
      if (!(numDots - 4)) draw_dots();
  }
}
