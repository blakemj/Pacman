#include "printf.h"
#include "uart.h"
#include "board.h"
#include "gl.h"
#include "pacman_char.h"
#include "timer.h"
#include "ghosts.h"

void main(void) 
{
  while (1) {
      board_init();
      draw_dots();
      pacman_init();
      ghosts_init();
      gl_swap_buffer();
      timer_delay(3);
      while(numDots - 4) {
          pacman_move();
          blinky_move();
          gl_swap_buffer();
      }
  }
}
