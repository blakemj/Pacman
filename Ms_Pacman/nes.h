#ifndef NES_H
#define NES_H

//This will initialize the nes controller
void nes_init(void);

//These will read the two nes controllers
void read_nes_controller();
void read_nes_controller_two();

//These keep track of the states of all 8 buttons on the first controller
volatile int a_button;
volatile int b_button;
volatile int select_button;
volatile int start_button;
volatile int up_button;
volatile int down_button;
volatile int left_button;
volatile int right_button;

//These keep track of the states of all 8 buttons on the second controller
volatile int msa_button;
volatile int msb_button;
volatile int msselect_button;
volatile int msstart_button;
volatile int msup_button;
volatile int msdown_button;
volatile int msleft_button;
volatile int msright_button;

#endif
