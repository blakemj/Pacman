#include "printf.h"
#include "uart.h"
#include "board.h"
#include "gl.h"

void main(void) 
{
  board_init();
  draw_dots();
  gl_swap_buffer();
}
