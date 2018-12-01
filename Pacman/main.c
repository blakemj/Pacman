#include "printf.h"
#include "uart.h"
#include "board.h"
#include "gl.h"
#include "pacman_char.h"

void main(void) 
{
  board_init();
  draw_dots();
  pacman_init();
  gl_swap_buffer();
  while(1) {
      pacman_move();
  }
}
