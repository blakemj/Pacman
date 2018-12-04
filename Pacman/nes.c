#include "nes.h"
#include "printf.h"
#include "gpio.h"
#include "gpioextra.h"

const unsigned int CLK  = GPIO_PIN20;
const unsigned int DATA = GPIO_PIN21;
const unsigned int LATCH = GPIO_PIN19;

void wait_for_falling_clock_edge() {
    while (gpio_read(CLK) == 0) {}
    while (gpio_read(CLK) == 1) {}
}

void nes_init(void) 
{
    gpio_set_output(CLK); 
 
    gpio_set_input(DATA); 
    gpio_set_pullup(DATA);

    gpio_set_output(LATCH);
}

int a_button;
int b_button;
int select_button;
int start_button;
int up_button;
int down_button;
int left_button;
int right_button;

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

