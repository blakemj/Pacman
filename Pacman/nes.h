#ifndef NES_H
#define NES_H

void nes_init(void);
void read_nes_controller();

volatile int a_button;
volatile int b_button;
volatile int select_button;
volatile int start_button;
volatile int up_button;
volatile int down_button;
volatile int left_button;
volatile int right_button;

#endif
