#ifndef NES_H
#define NES_H

void nes_init(void);
void read_nes_controller();
void read_nes_controller_two();

volatile int a_button;
volatile int b_button;
volatile int select_button;
volatile int start_button;
volatile int up_button;
volatile int down_button;
volatile int left_button;
volatile int right_button;

volatile int msa_button;
volatile int msb_button;
volatile int msselect_button;
volatile int msstart_button;
volatile int msup_button;
volatile int msdown_button;
volatile int msleft_button;
volatile int msright_button;

#endif
