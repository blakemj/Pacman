#include "nes.h"
#include "printf.h"
#include "gpio.h"
#include "gpioextra.h"

//These define the GPIO pins that the P1 controller is hooked up to
const unsigned int CLK  = GPIO_PIN20;
const unsigned int DATA = GPIO_PIN21;
const unsigned int LATCH = GPIO_PIN19;

//These define the GPIO pins that the P2 controller is hooked up to
const unsigned int CLK2  = GPIO_PIN27;
const unsigned int DATA2 = GPIO_PIN17;
const unsigned int LATCH2 = GPIO_PIN22;

/*
* This will initialize the controllers by setting the correct pins to input/output
*/
void nes_init(void) 
{
    gpio_set_output(CLK);
    gpio_set_output(CLK2); 
 
    gpio_set_input(DATA); 
    gpio_set_pullup(DATA);
    gpio_set_input(DATA2);
    gpio_set_pullup(DATA2);

    gpio_set_output(LATCH);
    gpio_set_output(LATCH2);
}

//See header
volatile int a_button;
volatile int b_button;
volatile int select_button;
volatile int start_button;
volatile int up_button;
volatile int down_button;
volatile int left_button;
volatile int right_button;

//See header
volatile int msa_button;
volatile int msb_button;
volatile int msselect_button;
volatile int msstart_button;
volatile int msup_button;
volatile int msdown_button;
volatile int msleft_button;
volatile int msright_button;

/*
* This will read the bits for controller 2--This will write a high to the latch, which will signal the controller
* to get the state of the buttons, which are all 8 stored in a scancode. This will then read the first bit and store
* it in the correct variable, then will send a high then a low to the clock to signal the controller to send the next
* bit. This does this in a loop for all 8 bits.
*/
void read_nes_controller_two() {
    gpio_write(LATCH2, 1);
    gpio_write(LATCH2, 0);
    for(int i = 0; i < 8; i++) {
        int data = gpio_read(DATA2);
        if (i == 0) msa_button = data;
        if (i == 1) msb_button = data;
        if (i == 2) msselect_button = data;
        if (i == 3) msstart_button = data;
        if (i == 4) msup_button = data;
        if (i == 5) msdown_button = data;
        if (i == 6) msleft_button = data;
        if (i == 7) msright_button = data;
        gpio_write(CLK2, 1);
        gpio_write(CLK2, 0);
    }
}

/*
* See read_nes_contoller_two()
*/
void read_nes_controller() {
    gpio_write(LATCH, 1);
    gpio_write(LATCH, 0);
    for(int i = 0; i < 8; i++) {
        int data = gpio_read(DATA);
        if (i == 0) a_button = data;
        if (i == 1) b_button = data;
        if (i == 2) select_button = data;
        if (i == 3) start_button = data;
        if (i == 4) up_button = data;
        if (i == 5) down_button = data;
        if (i == 6) left_button = data;
        if (i == 7) right_button = data;
        gpio_write(CLK, 1);
        gpio_write(CLK, 0);
    }
} 

