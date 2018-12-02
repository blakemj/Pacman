#include "printf.h"
#include "uart.h"
#include "board.h"
#include "gl.h"
#include "pacman_char.h"
#include "timer.h"

void main(void) 
{
  board_init();
  draw_dots();
  pacman_init();
  gl_swap_buffer();
  timer_delay(3);
  while(1) {
      pacman_move();
  }
}
